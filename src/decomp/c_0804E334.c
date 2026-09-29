#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E334.
 * sub_0804E334 @ 0x0804E334
 */

/* Re-bases a stashed OBJ tile index against the slot's current one and, when it
 * has moved and the slot really belongs to this side, records the delta in the
 * slot's unk28.
 *
 * THE PARAMETER IS NOT A `struct OamData *`. The ROM reads +4 with a plain
 * `ldrh`, subtracts, and masks the DIFFERENCE with 0x3ff; and it re-inserts with
 * `movs r0, #0xfc; lsls #8; ands` -- a 16-bit 0xfc00 mask and an `adds`.
 * Through OamData's `tileNum : 10` bitfield agbcc extracts the field on the read
 * side too and masks with the 32-bit 0xfffffc00 on the write side, which is four
 * bytes wrong. The stack copy CopySlotSpriteAttrs fills IS an OamData -- its tileNum
 * read is the `lsls #0x16; lsrs #0x16` extract -- so the two objects are
 * deliberately typed differently here.
 *
 * The `-` and `&` are folded: masking the difference with 0x3ff makes the
 * high bits of the left operand irrelevant, so agbcc never masks it. */
struct Unk4E334
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ u16 attr2;
};

void sub_0804E334(s16 a, struct Unk4E334 *p)
{
    struct OamData oam;
    u16 d;
    u16 g;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);

    d = (p->attr2 - oam.tileNum) & 0x3ff;
    g = gUnknown_03001470[a].unk30;
    p->attr2 = (p->attr2 & 0xfc00) + oam.tileNum;

    if (d != gUnknown_03001470[a].unk28 && a == gUnknown_02029668[g][3])
        gUnknown_03001470[a].unk28 = d;
}
