#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080315E8.
 * sub_080315E8 @ 0x080315E8
 */

/* Redraws the 0x12-wide window when the requested index differs from the
 * current one, and returns the requested index either way.
 *
 * THE RESULT IS AN `int` LOCAL, not the u16 parameter returned directly. With
 * `return b;` gcc keeps a second pseudo for the return value (an extra
 * `adds r6,r5,#0` and an extra pushed register) AND fully narrows `a` for the
 * compare; with the int local it narrows only `b`, and the comparison stays on
 * the two left-shifted parameters -- `cmp r0, r1` with neither `lsrs`, which is
 * shorten_compare on an equality test and is exactly what the ROM has. */
u16 LinkScreenSetMessage(u16 a, u16 b, int c)
{
    int r;

    r = b;

    if (a != b)
    {
        FillTilemapRect(gBG0TilemapBuffer, 0, 0x12, 0x20, 2, 0);
        StartTextBoxViaRecord(0, 0x12, gBG0TilemapBuffer, gUnknown_0849B0E2[r], 0x3000, 0x40);
    }

    return r;
}
asm(".global sub_080315E8\n.thumb_set sub_080315E8, LinkScreenSetMessage\n");
