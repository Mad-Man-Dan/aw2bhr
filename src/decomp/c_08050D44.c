#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050D44.
 * sub_08050D44 @ 0x08050D44
 */

/* Wave 85 (W85-D). Two constructs, both imported from the promoted twin
 * src/decomp/c_08050E08.c, which closed this exact residual class in wave 63:
 *
 * 1. The tail: each u16 memory operand of the two call additions is bound to an
 *    int local INSIDE the addition (mem_x/mem_y). That preserves the ldrh,
 *    gives the operand SImode precision and keeps the s16 local's explicit sign
 *    extension AHEAD of the load -- the ROM's order. Recovers both missing
 *    lsls/asrs pairs (wave 34/36/59 residual).
 *
 * 2. The middle: the cell address is bound E08-style with the k term FIRST --
 *    new_var = &((...)((u8 *)row) + (k * 4 + col * 0x10))->unk00 -- and the row
 *    address itself is spelled OFFSET-FIRST, BASE-LAST (gUnknown_03004580[a][1]
 *    * 0x30 + (u32)tbl), which is what turns the subscript spelling's
 *    `add r1, r6, r1` into the ROM's `add r1, r1, r6`. Same principle as the
 *    E7A8 entry byte-offset sum: the ROM adds the base last. */
void PlaceThirdEffectOnFigure(u16 a, u16 b)
{
    struct Unk08553B1CPt (*tbl)[3][4];
    struct Unk08553B1CPt (*row)[4];
    s16 *new_var;
    int k;
    s16 x;
    s16 y;
    int mem_x;
    int mem_y;

    tbl = gUnknown_08553B1C[gUnknown_020296B0[a].unk1c];

    k = gUnknown_08562128[gUnknown_03004580[gUnknown_0300453C][4]];
    if (k == 4)
        k = 2;

    row = (struct Unk08553B1CPt (*)[4])(gUnknown_03004580[a][1] * 0x30 + (u32)tbl);

    new_var = &((struct Unk08553B1CPt *)(((u8 *)row) + ((k * sizeof(struct Unk08553B1CPt)) + (gUnknown_03004580[a][2] * 0x10))))->unk00;
    x = *new_var;
    y = ((struct Unk08553B1CPt *)(((u8 *)row) + ((k * sizeof(struct Unk08553B1CPt)) + (gUnknown_03004580[a][2] * 0x10))))->unk02;

    if (a != 0)
        x = -x;

    SetEffectScreenPosition(a, gUnknown_03001FBC,
                 x + (mem_x = gUnknown_02029A10[a].entries[b].x),
                 y + (mem_y = gUnknown_02029A10[a].entries[b].y));
}
asm(".global sub_08050D44\n.thumb_set sub_08050D44, PlaceThirdEffectOnFigure\n");
