#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087B74.
 * sub_08087B74 @ 0x08087B74, sub_08087C14 @ 0x08087C14
 */

#include "proc.h"
/* Redraws the roster row for the unit list at gUnknown_02027F74.unk04[a]:
 * uploads each unit's tile blob into OBJ VRAM and, for the ones IsCampaignMapUnlocked
 * rejects, overwrites their 16-colour palette. The twin of LoadMapRecordMinimugs, which
 * reaches the same byte through the overlapping gUnknown_02027F78 extern.
 *
 * THE `+ 1` AND THE TABLE HOIST ARE ONE FACT, and wave 44 parked this function
 * on them. Both fall out of writing the two loop-invariant pieces as ORDINARY
 * STATEMENTS INSIDE THE LOOP BODY:
 *
 *  - `k = gUnknown_02027F74.unk04[a] * 0x5c + 1;` in its own statement stops
 *    fold re-associating the `+ 1` past the loop counter. Written inline as
 *    `[v * 0x5c + 1 + i]` the constant migrates to the end of the address sum
 *    and lands in the pool word's ADDEND or the load displacement
 *    (`ldrb r0, [r0, #1]`), where the ROM has a clean `.4byte gUnknown_085C77DC`
 *    and a runtime `adds r0, #1`. `k` is dead at the end of each iteration, so
 *    it costs no register -- it is punctuation, not storage.
 *  - `tbl = (u8 *)gUnknown_085C77DC;` INSIDE the loop is hoisted by LICM into
 *    the preheader, which places `ldr r0, =gUnknown_085C77DC; mov sb, r0` AFTER
 *    the `cmp r5, r7; bge` guard and BEFORE the giv init -- the ROM's order.
 *    Assigning it before the loop makes it a source statement and it lands two
 *    instructions early, ahead of `str r5, [r0, #0x54]`; naming the array
 *    directly does not hoist at all and re-loads the pool word every iteration.
 *    This is the brief's preheader rule read forwards: source, then LICM, then
 *    strength_reduce.
 *
 * The index add must be spelled `i + k`, not `k + i`. Both are the same value
 * and the same instruction count, but THUMB `ADDS Rd, Rn, Rm` encodes the two
 * source registers in different fields, so `k + i` emits `adds r0, r0, r5`
 * where the ROM has `adds r0, r5, r0` -- two differing bytes and nothing else.
 *
 * Also settled and unchanged from wave 44's reading:
 *  - `gUnknown_02027F74.unk04[a]` written out at all three uses; CSE keeps the
 *    address in r6 and re-`ldrb`s across the calls, which is the ROM.
 *  - `i + 0x17` passed INLINE as LoadCoMiniPortrait's third argument; as a statement
 *    it is computed ahead of argument 1. CSE still carries it in r4 across the
 *    call for the ApplyPaletteExt use.
 *  - `lsls #0x15; lsrs #0x10` is `(u16)(pal * 0x20)`, a u16 truncation of the
 *    PRODUCT, not a mask and a shift.
 *  - `0x90 + i * 0xc` is a strength-reduction giv and must not be authored as
 *    an accumulator.
 *  - the `lsls #0x18` after IsCampaignMapUnlocked is the caller's own `(u8)` cast; the
 *    promoted src/decomp/c_0803CA54.c defines an `int` return. */

struct Unk08087B74Proc
{
    PROC_HEADER;
    STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
};
/* Redraws five OBJ tile blobs for the unit gUnknown_02027F78[a], skipping the
 * slots whose 12-bit middle bitfield is zero, into the proc found by script
 * gUnknown_08616DB4 (whose +0x54 word it first seeds with `a`).
 *
 * src/decomp/c_08087B60.c already read this function's shape from its caller:
 * `a` is an INT used as an offset into a global's address, and it is stashed at
 * +0x54 of the proc found by script. This is the twin of LoadEnemyCoMinimugs, which
 * reaches the same byte as gUnknown_02027F74.unk04[a] instead -- the two symbols
 * overlap (0x02027F78 == gUnknown_02027F74 + 4) and each function names the one
 * its own CLEAN pool word records: `.4byte gUnknown_02027F78` here against
 * `.4byte gUnknown_02027F74` plus a runtime `adds #4` there.
 *
 * `gUnknown_02027F78[a]` is written INSIDE the loop, not bound to a pointer
 * before it. That is what puts its address computation (`ldr r0, =gUnknown_02027F78;
 * adds r6, r5, r0`) in the preheader AFTER the `movs r4, #0` for-init, where
 * only LICM writes; a source-level binding lands ahead of the for-init instead.
 * The same reasoning puts the gUnknown_0200C078 base there.
 *
 * The record is struct Unk0200C078Rec, the 8/12/12 bitfield word wave 38 proved:
 * `ldr; lsls #0xc; lsrs #0x14` is unk00_08 (bits 8..19) read as the guard and
 * `ldrb` off the SAME address is unk00_00 (bits 0..7) handed to LoadCoMiniPortrait.
 * Writing the subscript out twice is what CSEs the address into r1 across both.
 *
 * The flat index `[v - 0x6c].unk00[i]` is the 0x14-byte row form and it emits
 * `((v - 0x6c) * 5 + i) * 4`, i.e. the row multiply folded with the element
 * index before the scale -- which is the ROM. The 0x6c bias also fixes the row
 * range at 0..29, matching the declared extent of gUnknown_0200C078[30]. */

struct Unk08087C14Proc
{
    PROC_HEADER;
    STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
};

void LoadEnemyCoMinimugs(int a)
{
    struct Unk08087B74Proc *proc;
    u8 *tbl;
    int n;
    int i;
    int k;

    n = GetMapArmyCount(gUnknown_02027F74.unk04[a]);
    proc = Proc_Find(ProcScr_PutEnemyCoMinimug);

    if (proc != NULL)
    {
        proc->unk54 = a;

        for (i = 0; i < n - 1; i++)
        {
            tbl = (u8 *)gUnknown_085C77DC;
            k = gUnknown_02027F74.unk04[a] * 0x5c + 1;

            LoadCoMiniPortrait(tbl[i + k],
                (void *)(((0x90 + i * 0xc) & 0x3FF) * 0x20 + 0x06010000), i + 0x17);

            if ((u8)IsCampaignMapUnlocked(gUnknown_02027F74.unk04[a]) == 0)
                ApplyPaletteExt(gUnknown_08614238, (u16)((i + 0x17) * 0x20), 0x20);
        }
    }
}
asm(".global sub_08087B74\n.thumb_set sub_08087B74, LoadEnemyCoMinimugs\n");

void LoadMapRecordMinimugs(int a)
{
    struct Unk08087C14Proc *proc;
    int i;

    proc = Proc_Find(gUnknown_08616DB4);

    if (proc != NULL)
    {
        proc->unk54 = a;

        for (i = 0; i < 5; i++)
        {
            if (gUnknown_0200C078[gUnknown_02027F78[a] - 0x6c].unk00[i].unk00_08 != 0)
                LoadCoMiniPortrait(gUnknown_0200C078[gUnknown_02027F78[a] - 0x6c].unk00[i].unk00_00,
                    (void *)(((0xb4 + i * 0xc) & 0x3FF) * 0x20 + 0x06010000),
                    i + 0x1a);
        }
    }
}
asm(".global sub_08087C14\n.thumb_set sub_08087C14, LoadMapRecordMinimugs\n");
