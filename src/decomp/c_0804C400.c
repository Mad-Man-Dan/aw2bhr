#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C400.
 * sub_0804C400 @ 0x0804C400
 */

/* The position table GetFigurePositionTable hands back is addressed with EXPLICIT BYTE
 * arithmetic, and the exact parenthesisation is load-bearing -- three separate
 * spellings of the same address each moved one instruction:
 *
 *   ((struct Pos *)p)[a * 5 + i]   keeps `p` in its own register (the ROM's
 *       `adds r6, r0, #0` and the five-register push) but hoists the row offset
 *       UNSCALED and applies one shared `* 4` to the sum -- one instruction
 *       short in the loop preheader.
 *   p + a * 20 + i * 4            scales the row offset correctly but lets
 *       agbcc fold `p + a * 20` into a single invariant register, dropping to a
 *       four-register push. Same for `((struct Pos (*)[5])p)[a][i]` and for a
 *       `q` bound inside the loop body.
 *   p + (a * 20 + i * 4)          the parentheses keep `p` out of the invariant
 *       AND keep both terms scaled independently, which is the ROM.
 *
 * The two `+ 2` reads fold into the `ldrh` displacement. Each source halfword
 * is loaded twice because the `strh` into gUnknown_02029A10 can alias it -- both
 * are u16 objects in RAM -- so agbcc cannot carry the value across the store. */
void SpawnWholeFigure(u16 a)
{
    u32 p;
    u16 i;

    StartWholeFigureSlot(a, gUnknown_08552148[a]);

    p = GetFigurePositionTable(gUnknown_085D6A48[gUnknown_03004580[a][1]][0],
        gUnknown_03004580[a][3]);

    for (i = 0; i < 5; i++)
    {
        gUnknown_02029A10[a].entries[i].x = *(u16 *)(p + (a * 20 + i * 4));
        gUnknown_02029A10[a].entries[i].unk04 = *(u16 *)(p + (a * 20 + i * 4));
        gUnknown_02029A10[a].entries[i].y = *(u16 *)(p + (a * 20 + i * 4) + 2);
        gUnknown_02029A10[a].entries[i].unk06 = *(u16 *)(p + (a * 20 + i * 4) + 2);
    }
}
asm(".global sub_0804C400\n.thumb_set sub_0804C400, SpawnWholeFigure\n");
