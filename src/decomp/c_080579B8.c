#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080579B8.
 * sub_080579B8 @ 0x080579B8
 */

/* gUnknown_085D6A48's 24-byte rows are STRUCTS, not `u16[12]`: the column-1
 * offset stays in the `ldrh` displacement instead of being reassociated onto
 * the base.  See the "column-offset fold" chapter of docs/agbcc-codegen.md.
 * The global keeps its `u16 [][12]` declaration (wave 17 recorded why), so the
 * row shape is spelled locally and cast at the use.  The cast has to sit INSIDE
 * the loop: written before the `for`, the address load lands ahead of `i = 0`
 * in the preheader, and written as one inline expression `-fforce-addr` parks
 * the base in this unit's .rodata and adds an `ldr`. */
struct Unk085D6A48Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x14];
};

void DrawBothSideHud(u16 *dst)
{
    struct Unk085D6A48Row *rows;
    int pos;
    int i;
    int idx;

    for (i = 0; i <= 1; i++)
    {
        rows = (struct Unk085D6A48Row *)gUnknown_085D6A48;
        idx = rows[gUnknown_03004580[i][1]].unk02 * 2 + i;
        pos = i << 19;
        sub_080576D4(dst, idx, (struct Unk8057Pos *)&pos);
        DrawHpGaugeStrip(dst, idx, (struct Unk8057Pos *)&pos);
        DrawHpNumber(dst, idx, (struct Unk8057Pos *)&pos);
        sub_08057860(dst, idx, (struct Unk8057Pos *)&pos);
    }
}
asm(".global sub_080579B8\n.thumb_set sub_080579B8, DrawBothSideHud\n");
