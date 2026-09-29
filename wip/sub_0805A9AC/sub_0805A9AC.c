#include "global.h"


/* PARKED, wave 56 W56-M. Wave 62 remeasured the configured draft at the same
 * 35.7%, candidate 760 bytes vs 732 (+28), correcting the queue's provisional
 * +24 label. `no-force` removes most of the surplus but over-corrects to +8
 * (740 bytes, 22.1%); `o1-no-force` is -12 and unrelated. Volatile reads of
 * the map pointer throughout the inner nest worsen configured to +68, proving
 * that suppressing all CSE is not the missing selective rematerialisation.
 * The original readable +28 draft is retained.
 *
 * THE +28 IS ONE FACT, NOT FOUR. This candidate force-addrs gUnknown_08499590
 * a FOURTH time and the ROM does not. The extra `.rodata` word's value is kept
 * live in sl across the whole i/j nest (`ldr r2,[pc]` hoisted into the j
 * preheader, then `ldr r2,[r2]; mov sl,r2; ldr r3,[r2]` -- the full three-level
 * chain), which is one live value more than the ROM carries. That spills `i`
 * to a stack slot (+4 frame, `sub sp,#40` vs `#36`), which pushes `best` out
 * of r6 into r8 (+4 in high-register shuffles around the `ands`/`orrs` insv),
 * which stops jump.c cross-jumping the i-loop entry onto the shared bottom
 * test (+6), plus the extra pool word (+4). Fix the force-addr and the rest
 * should fall out together -- do NOT chase them separately.
 *
 * The ROM instead rematerialises &gUnknown_08499590 with a plain inline pool
 * word at each of its four in-loop sites and lets r7 die at the i preheader.
 * Per the wave-18 rule this is register pressure in the C, not a park -- but I
 * did not find the surplus value. Ruled out by measurement, not by argument:
 *   - the unk14 COMPONENT_REF fix below (real, keep it: without it the +0x1a
 *     folds into the `ldrb` displacement where the ROM has `adds r1,#26`, and
 *     the unk14 pointer load is deferred past the terrain read) moved 35.0 ->
 *     35.7% and did NOT change the size, so it is not the cause;
 *   - the unk09 bitfield below is also real and also did not change the size
 *     (22.7 -> 35.0%).
 *   - Wave 66 made only the j-loop bound read of gUnknown_08499590 volatile,
 *     the narrowest selective-rematerialisation probe. It compiled
 *     byte-identically to this +28, 35.7% draft, so that access is not the
 *     pseudo keeping the force-addr value live.
 *   - Wave 72 made only the inner `unk12` read use a volatile view of the map
 *     pointer. It remains 760/732 (+28), but improves configured identity to
 *     37.6%, so that strongest draft is retained below. Volatile views of all
 *     `unk1432`, `unk2D5A`, or `unk417A` reads regress to +32/35.0%,
 *     +32/27.0%, and +68/27.3%; combining `unk12` with `unk1432` is +40/35.8%.
 *     Selective volatility can rotate the allocation, but none removes the
 *     surplus force-address word or spill cascade.
 *
 * SETTLED AND WORTH KEEPING, all read off the assembly:
 *   - unk09 bits 3..5 is a BITFIELD, not `(x >> 3) & 7`. The ROM's
 *     `lsls #0x1a; lsrs #0x1d` is an extzv; the shift-and-mask spelling emits
 *     `lsrs #3; movs #7; ands`. Same carve as c_08059674.c's Unk0805DFF4Rec.
 *   - gUnknown_0816D9AC is an 8-byte ROM TEMPLATE copied to a `u8 t[2][4]`
 *     stack buffer, and the copy is IN THE SOURCE: the ROM relocates against
 *     sub_0808B6E8, the game's own copier, where an auto aggregate's
 *     initialiser would have gone through agbcc's `memcpy`. See the note in
 *     include/unknown-globals.h.
 *   - bv/bestv are `s16`: PROMOTE_MODE zero-extends the store
 *     (`lsls #16; lsrs #16`) and every read sign-extends.
 *   - `best` is a `union Unk802C57CBuf` in one register: `best.pos.unk00 =
 *     0x270f` is an insv (`ands` 0xFFFF0000 / `orrs`), the full write is
 *     `best.raw = (u16)j | (i << 16)`.
 *   - gUnknown_085D5AD0 is &gUnknown_085D5ABC[0].unk14; the candidate's
 *     `gUnknown_085D5ABC` + addend 0x14 is the identical address (benign).
 */

struct Map5A9AC
{
    /* 0x0000 */ u16 unk00;
    /* 0x0002 */ u16 unk02;
    /* 0x0004 */ u8 filler_04[0x000E];
    /* 0x0012 */ u8 unk12[0x1420];
    /* 0x1432 */ u8 unk1432[0x1928];
    /* 0x2d5a */ u8 unk2D5A[0x0508];
    /* 0x3262 */ u8 unk3262[0x0F18];
    /* 0x417a */ u16 unk417A[0x100];
};

struct Unk5A9ACTbl
{
    /* 0x00 */ u8 filler_00[0x1a];
    /* 0x1a */ u8 unk1a[0x20];
};

struct Unk5A9ACUnit
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};

void sub_0805A9AC(int a1, void *a2)
{
    struct Unk802C57C *cur;
    union Unk802C57CBuf best;
    struct Unit *p;
    u8 t[2][4];
    s16 bv;
    s16 bestv;
    int n;
    struct Map5A9AC *new_var;
    int i;
    int j;
    int nj;
    u8 tile;

    cur = a2;
    bv = 0x7fff;
    sub_0808B6E8(t, gUnknown_0816D9AC, 8);
    sub_0801F92C(gUnknown_08499590 + 0x2852);
    sub_08058224((struct Unit *)gUnknown_030040D8);

    for (n = gUnknown_03003F2C; n < gUnknown_03003F2C + 0x40; n++)
    {
        p = &gUnknown_08499594[n];
        if (p->type == 0)
            continue;
        if (p->type > 2)
            continue;
        if (gUnknown_03004730[n & 0x3f] != 0)
            continue;
        if (t[a1][((struct Unk5A9ACUnit *)p)->unk09_3] == 0)
            continue;
        if ((s8)((struct Map5A9AC *)gUnknown_08499590)->unk2D5A[
                ((struct Map5A9AC *)gUnknown_08499590)->unk417A[p->y] + p->x] > bv)
            continue;

        gUnknown_030013EC(p->x, p->y, 1,
                          sub_08042D1C(gUnknown_030033EC, p->type), -1);

        bestv = 0x7fff;
        best.pos.unk00 = 0x270f;

        for (i = 0; i < ((struct Map5A9AC *)gUnknown_08499590)->unk02; i++)
        {
            for (j = 0; j < ((struct Map5A9AC *)gUnknown_08499590)->unk00; j = nj)
            {
                nj = j + 1;
                new_var = (struct Map5A9AC *)gUnknown_08499590;
                if ((s8)gUnknown_03003340[i][j] < 0)
                    continue;
                if ((s8)new_var->unk2D5A[new_var->unk417A[i] + j] > bestv)
                    continue;
                if ((*(struct Map5A9AC * volatile *)&gUnknown_08499590)->unk12[
                        ((struct Map5A9AC *)gUnknown_08499590)->unk417A[i] + j] != 0)
                    continue;
                if (((struct Unk5A9ACTbl *)gUnknown_085D5ABC[gUnknown_030040D8->unk00].transportTable)->unk1a[
                        ((struct Map5A9AC *)gUnknown_08499590)->unk1432[new_var->unk417A[i] + j] & 0x1f] == 0)
                    continue;
                tile = ((struct Map5A9AC *)gUnknown_08499590)->unk1432[
                    ((struct Map5A9AC *)gUnknown_08499590)->unk417A[i] + j];
                if (sub_08026FD0(gUnknown_03003F38, tile) == 1
                 && gUnknown_085767B8[((struct Map5A9AC *)gUnknown_08499590)->unk1432[new_var->unk417A[i] + j] & 0x1f] != 0)
                    continue;
                best.raw = (u16)j | (i << 16);
                bestv = (s8)((struct Map5A9AC *)gUnknown_08499590)->unk2D5A[new_var->unk417A[i] + j];
            }
        }

        if (best.pos.unk00 == 0x270f)
            continue;
        cur->unk00 = best.pos.unk00;
        cur->unk02 = best.pos.unk02;
        bv = (s8)((struct Map5A9AC *)gUnknown_08499590)->unk2D5A[((struct Map5A9AC *)gUnknown_08499590)->unk417A[best.pos.unk02] + best.pos.unk00];
    }

    if (cur->unk00 != 0x270f)
        gUnknown_03004730[((struct Map5A9AC *)gUnknown_08499590)->unk12[
            ((struct Map5A9AC *)gUnknown_08499590)->unk417A[cur->unk02] + cur->unk00] & 0x3f]++;
}















