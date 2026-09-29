#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805601C.
 * sub_0805601C @ 0x0805601C
 */

/* PlanSideDeaths @ 0x0805601C */

/* The four `cmp`s against 4, 6, 1, 7 are tested in that order with no table and
 * no sorting, which is a chain of `||` in source order and not a switch.
 *
 * gUnknown_085D6A48's column 3 needs the `struct Unk85D6A48Row *` local, not
 * the nested `gUnknown_085D6A48[X][3]`: the array form emits `adds rB, #6` in
 * place of the ROM's `ldrh r0, [r0, #6]` displacement, while a folded
 * `*(gUnknown_085D6A48[X] + 3)` moves the +6 into the POOL WORD
 * (`.word gUnknown_085D6A48+0x6`) and frees a register, dropping r7 from the
 * push. Binding the base to the local is what gives the displacement AND keeps
 * the bare pool word loaded first. See the note in DrawTileBlock5x5B, where the same
 * table needs the comma-operator form of the same anchor.
 */
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u8 filler_08[0x12 - 0x08];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[0x18 - 0x14];
};

void PlanSideDeaths(u16 a, u16 b, u16 c, u16 d)
{
    struct Unk85D6A48Row *rows;
    u16 t;

    if (gUnknown_03004580[a][2] == 1)
    {
        rows = (struct Unk85D6A48Row *)gUnknown_085D6A48;
        t = rows[gUnknown_03004580[a][1]].unk06;

        if (t == 4 || t == 6 || t == 1 || t == 7)
            sub_080560A4(b, a ^ 1, c, 2, d);
        else
            sub_0805634C(b, a ^ 1, c);
    }
    else
    {
        sub_0805634C(b, a ^ 1, c);
    }
}
asm(".global sub_0805601C\n.thumb_set sub_0805601C, PlanSideDeaths\n");
