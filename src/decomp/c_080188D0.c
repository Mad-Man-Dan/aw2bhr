#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080188D0.
 * sub_080188D0 @ 0x080188D0
 */

/* EventOp_SetFramePaletteSlot15's conditional twin: the army whose palette is repainted comes
 * from the script node's own unk08 instead of gUnknown_030033EC, and 5 stands
 * in when that field is zero.
 *
 * `subs r0, #1` is shared by both arms, so the `- 1` is applied ONCE to a
 * variable -- two separate calls would have folded `5 - 1` to `movs r0, #4`.
 *
 * The node's unk08 is declared `u16` and read `ldrsh` here, so it is cast at
 * the use, the same treatment it gets in c_08018DF8.c and c_08017C4C.c. Reading
 * it twice is what the ROM does: the else arm reloads it rather than keeping
 * the compare's value. */
bool8 EventOp_ApplyFramePaletteForArmy(s16 a)
{
    struct Unk0200C528Node *p;
    int v;

    if (gUnknown_03002514 != 1)
    {
        p = gUnknown_0200C528[a].unk04;
        if ((s16)p->unk08 == 0)
            v = 5;
        else
            v = gPlayers[(s16)p->unk08].teamColor;
        ApplyWindowFramePalette(v - 1, gUnknown_03002F08.unk00);
    }
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_080188D0\n.thumb_set sub_080188D0, EventOp_ApplyFramePaletteForArmy\n");
