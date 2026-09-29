#include "global.h"

/* gUnknown_08136060 and gUnknown_08136064 are `-fforce-addr` .rodata words
 * holding &gUnknown_02029A10 and &gUnknown_02029B94 (dumped from baserom.gba).
 *
 * The `= 0` writes inside the guarded block come out as the register that
 * already holds unk00, which `cmp r3,#0; beq` proved zero -- agbcc substituting
 * the compare's register, the same thing src/decomp/c_08003DC4.c records. */
extern struct Unk02029A10Group *const gUnknown_08136060;
extern u16 (*const gUnknown_08136064)[5];

void sub_0804CA98(u16 a1, u16 a2, s16 a3)
{
    struct Unk56E28 s;
    u16 cnt;
    struct Unk02029A10Group *base = gUnknown_08136060;

    if (base[a1].entries[a2].unk00 == 0)
    {
        u16 (*b94)[5] = gUnknown_08136064;

        base[a1].entries[a2].unk20 += b94[a1][a2];
        if (((*(volatile u16 *)&base[a1].entries[a2].unk20) & 0xf)
            == 1)
        {
            base[a1].entries[a2].unk22++;
            if (base[a1].entries[a2].unk22 == 3)
            {
                b94[a1][a2] = 0;
                base[a1].entries[a2].unk20 = 0xff;
            }
            cnt = base[a1].entries[a2].unk22;
            base[a1].entries[cnt].x =
                base[a1].entries[gUnknown_08552148[a1]].x
                + ((u16 *)gUnknown_0855335C)[a1 * 10 + cnt * 2];
            base[a1].entries[cnt].y =
                base[a1].entries[gUnknown_08552148[a1]].y
                + ((u16 *)gUnknown_0855335C)[a1 * 10 + cnt * 2 + 1];
        }
        if (gUnknown_02029B80[a1][a2] != 0)
        {
            gUnknown_02029B80[a1][a2] = 0;
            b94[a1][a2] = 1;
            ((struct Unk02029A10 *)(a2 * sizeof(struct Unk02029A10)
                + a1 * sizeof(struct Unk02029A10Group)
                + (u8 *)base))->frame = 0;
            s.unk00 = a1;
            s.unk02 = a2;
            s.unk04 = ((u16 *)gUnknown_08553354)[a1 * 2];
            s.unk06 = ((u16 *)gUnknown_08553354)[a1 * 2 + 1];
            s.unk08 = 0;
            s.unk0a = 0;
            s.unk0c = 0x78;
            sub_08056E28(&s);
        }
        if (((struct Unk02029A10 *)(a2 * sizeof(struct Unk02029A10)
                 + a1 * sizeof(struct Unk02029A10Group)
                 + (u8 *)gUnknown_02029A10))->frame
                == ((struct Unk02029A10 *)(a2 * sizeof(struct Unk02029A10)
                 + a1 * sizeof(struct Unk02029A10Group)
                 + (u8 *)gUnknown_02029A10))->frameCount
            && a3 != -1 && sub_080153F0(a3))
            sub_08015328(a3);
    }
}

/* PARKED, wave 37 (W37-I), ONE attempt only -- the batch ran out of budget, so
 * this is a first draft and not an exhausted one. The whole tail (the
 * gUnknown_02029B80 block, the struct Unk56E28 fill and the frame ==
 * frameCount guard) is structurally right and reads instruction-for-
 * instruction against the ROM. What is wrong:
 *   - the frame is 28 bytes, the ROM's is 32: the ROM CSEs THREE index
 *     intermediates to the stack ([sp,#0x14] = a1 * 10, [sp,#0x18] = a1 * 4,
 *     [sp,#0x1c] = a2 * 2) and reuses them across gUnknown_0855335C,
 *     gUnknown_02029B80/B94 and gUnknown_08553354. This draft spills only two
 *     and keeps a1/a2 themselves in r8/r9, where the ROM has them in the low
 *     r6/r7 and puts `a1 * 0xb4` in ip and gUnknown_02029A10 in sl.
 *   - gUnknown_0855335C: the ROM computes a HALFWORD index
 *     `a1 * 10 + cnt * 2` and then `+1` for the second read, i.e. one shared
 *     subexpression. Declared `u16 [][5][2]` and written `[a1][cnt][0]` /
 *     `[a1][cnt][1]` this draft folds the `+2` onto the BASE instead
 *     (`adds r5,#2`). `u16 [][10]` with `[a1][cnt * 2]` / `[a1][cnt * 2 + 1]`
 *     is the spelling that should reproduce it.
 *   - gUnknown_08553354[a1][1] has the same disease (`adds r0,#2; add r0,sl`
 *     where the ROM has `lsls r0,r6,#1; adds r0,#1; lsls r0,#1; adds r0,r0,r2`,
 *     i.e. the flat `(a1 * 2 + 1)` index).
 * Wave 66 (W66-D) tried both flat-index changes, plus a read-site-only
 * volatile cast on unk20. Together they produce the ROM's 0x20-byte frame and
 * restore the post-store `ldrh`, but gUnknown_02029A10 remains a plain literal
 * instead of the ROM's -fforce-addr word and the reload rebuilds the entry
 * address. Keeping an explicit entry pointer avoids that rebuild but collapses
 * the frame back to 0x1c. The configured fixpoint remains +26 code / +20
 * section bytes; flat indexing is ruled out as a standalone fix.
 *
 * The struct Unk02029A10 members unk20/unk22 added for this function are
 * independent of all that and are believed right (see include/unknown-globals.h). */






/* WAVE 77 (W77-A).  Improved from +8 bytes / 14.2% to -12 bytes / 14.9%, and
 * the register assignment now agrees with the ROM where before it agreed with
 * almost nothing (r5 entry, r6 a1, r7 a2, r8 the gUnknown_02029B94 element
 * pointer, r9 the zero, ip = a1 * 180).
 *
 * WHAT MOVED IT -- reuse this, it is the region's idiom and sub_0804F3C8 two
 * functions away already carried it:
 *   - The guarded region's four gUnknown_02029A10 accesses are spelled
 *     `(struct Unk02029A10 *)(n * sizeof(struct Unk02029A10)
 *        + a1 * sizeof(struct Unk02029A10Group) + (u8 *)gUnknown_02029A10)`,
 *     not `gUnknown_02029A10[a1].entries[n]`.  The array form folds the base
 *     into each MEM address so agbcc re-materialises the literal per use; the
 *     `(u8 *)` cast as an operand of `+` creates ONE address-constant pseudo.
 *   - gUnknown_0855335C is read through a FLAT `u16 *` cast at
 *     `a1 * 10 + cnt * 2`, not `[a1][cnt][0]` on the declared `[][5][2]`.
 *     The flat form makes the CSEd subexpression `a1 * 10`, which is what the
 *     ROM parks at [sp,#20] and what the neighbouring gUnknown_02029B94 index
 *     then reuses; the 3-D form shares `a1 * 5` instead.
 *   - gUnknown_08553354 likewise flat at `a1 * 2` / `a1 * 2 + 1`.
 *
 * REMAINING, and it is one fact: the ROM parks &gUnknown_02029A10 in `sl`,
 * loaded from the `-fforce-addr` .rodata word gUnknown_08136060 before the
 * unk00 test, and reaches all four uses with `add rN, sl`.  This candidate
 * still materialises a plain literal at each of the four.  Everything else in
 * the diff (the a1*4 stack slot vs sl, the gUnknown_08553354 `(base+2) + a1*4`
 * refactor, the 0x1c vs 0x20 frame) is downstream of that one difference.
 *
 * MEASURED AND RULED OUT this wave:
 *   - Converting the TAIL's three gUnknown_02029A10 accesses to the same
 *     `(u8 *)` idiom: -36 bytes / 6.5%.  The ROM's pool carries BOTH
 *     gUnknown_08136060 and a plain gUnknown_02029A10 word; the tail
 *     deliberately recomputes from the plain literal.  Convert the region, not
 *     the function.
 *   - Binding the base to a local (`u8 *base = (u8 *)gUnknown_02029A10;`) and
 *     using `base` in the sums: BYTE-IDENTICAL.  gcc const-propagates it.  The
 *     documented `c_local` workaround does not reach this.
 *   - Binding `dst` to an explicit local: byte-identical (CSE already did it).
 *     Kept anyway, because it mirrors sub_0804F3C8 and reads better.
 * Residual kind 3 (allocation / constant placement).
 */

/* WAVE 77 (W77-H).  NOW SIZE-EXACT, 416/416, 345 of 416 bytes differ (17.1%),
 * first difference at +0xa.  Was -12 bytes / 14.9%.
 *
 * The W77-A note above says the guarded region must use the `(u8 *)g + n` form.
 * That is half right, and it is the half that costs the force-addr word:
 * `(u8 *)gUnknown_02029A10` decays to `&gUnknown_02029A10[0]`, an address-of,
 * and the -fforce-addr chapter's own rule is that `&gSym` is NOT a use.  So
 * that spelling can never emit gUnknown_08136060, which is why this candidate
 * was short by exactly the missing `ldr rP,=word; ldr rB,[rP]; mov sl,rB` plus
 * the pool word plus one reload.
 *
 * WHAT IS IN THE BODY NOW, and why each piece is there:
 *   - The guarded region is spelled BY NAME, `gUnknown_02029A10[a1].entries[n]`.
 *     Only a by-name memory reference can produce the force-addr word.
 *   - The TAIL (`entries[a2].frame = 0`, and the frame == frameCount guard) is
 *     spelled `(u8 *)g + n * sizeof`.  The ROM's pool carries BOTH a force-addr
 *     word and a plain gUnknown_02029A10 literal; the tail is where the plain
 *     one comes from.  This region/tail split is what took it from -4 bytes to
 *     size-exact.
 *   - The read-site `volatile` cast on unk20 is back (W66-D had it, the wave-77
 *     draft had lost it).  It restores the ROM's post-store `ldrh` reload, 2 B.
 *   - W77-A's flat table indices on gUnknown_0855335C and gUnknown_08553354 are
 *     KEPT.  The old "array form is +8 bytes" measurement was array form plus
 *     the 3-D table indices; the tables were the 8 bytes, not the array form.
 *
 * MEASURED AND RULED OUT this wave, in addition to W77-A's list:
 *   - region array + tail array: -4 bytes / 17.3%.
 *   - region `(u8 *)g + n` bound to an `entry` pointer local, with by-name array
 *     form only for the cnt / gUnknown_08552148 accesses: -36 bytes / 13.9%.  A
 *     pointer local converts every access under it into a non-use and takes the
 *     whole region's by-name count with it.
 *
 * REMAINING, one fact: the candidate still reaches gUnknown_02029A10 through the
 * plain literal in the guarded region, so gUnknown_08136060 is absent and the
 * plain word is created FIRST (pool `02029A10, .rodata, ...` against the ROM's
 * `.rodata, .rodata, ..., 02029A10`).  Everything else in the diff is downstream
 * of it.  Residual kind 3.  See the W77-H chapter in docs/agbcc-codegen.md. */

/* WAVE 78 (W78-B).  Configured fixpoint reverified size-exact 416/416,
 * 17.1%, 345 differing bytes, first difference +0xa.  Resumed only at W77-H's
 * named residual.  Converting any one or two of the tail's `frame` /
 * `frameCount` references back to array form is byte- and pool-neutral; doing
 * all three, then retaining one plain-literal use by converting one guarded
 * x/y/unk22 reference to `(u8 *)g + n`, also leaves the same single
 * gUnknown_02029A10 plain word and no force-address word.  Goto/early-return
 * forms for the outer guard and a goto for the inner cnt guard likewise leave
 * the pool unchanged.  These region splits do not change the surviving
 * by-name reference that the force-address pass sees. */



