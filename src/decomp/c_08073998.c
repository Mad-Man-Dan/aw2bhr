#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073998.
 * sub_08073998 @ 0x08073998
 */

/* Walks one edge of a polygon down the scanline table, handing each row to
 * SetWipeEdge.  The endpoints are sorted by y first, the slope is a 16.16
 * fixed-point `dx/dy` -- a real signed `/`, which is where __divsi3 comes from
 * (do not author the helper call) -- and x is stepped by it once per row.
 *
 * The swap's temporary takes the SECOND element first (`t = x2; x2 = x1;
 * x1 = t;`): the other order emits the same six moves with the operands of the
 * first two exchanged.
 *
 * The bottom clamp to 0xA0 and the top clamp to 0 are not symmetrical -- the
 * top one has to advance x by `slope * -y1` to keep the edge on the same line,
 * and it is spelled as a negate-then-multiply, not `x -= slope * y1`.
 *
 * gUnknown_0202FDE0 is re-loaded from the global inside the loop because
 * SetWipeEdge may store through it; only the address is hoisted (r8). */

void DrawWipeEdgeLine(int x1, int y1, int x2, int y2, int c)
{
    int slope;
    int x;
    int t;

    if (y1 > y2)
    {
        t = x2;
        x2 = x1;
        x1 = t;
        t = y2;
        y2 = y1;
        y1 = t;
    }

    slope = ((x2 - x1) << 16) / (y2 - y1);
    x = x1 << 16;

    if (y2 > 0xA0)
        y2 = 0xA0;

    if (y1 < 0)
    {
        x += slope * -y1;
        y1 = 0;
    }

    for (; y1 < y2; y1++)
    {
        SetWipeEdge(x >> 16, y1, c, gUnknown_0202FDE0);
        x += slope;
    }
}
asm(".global sub_08073998\n.thumb_set sub_08073998, DrawWipeEdgeLine\n");
