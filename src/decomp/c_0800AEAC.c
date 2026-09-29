#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800AEAC.
 * sub_0800AEAC @ 0x0800AEAC
 */

/*
 * CanPlaceRiverAt -- may sea be placed at (x, y)? 1 means yes.
 *
 * sub_0800A6AC gives the cell's land shape, and a negative answer refuses
 * outright. A non-zero shape is accepted when sub_0800A95C reports nothing
 * (0), or, when it reports a shape of its own, only if the top bits of the land
 * shape are 0x4000, 0x2000 or 0. A zero shape falls back on sub_0800A884 and is
 * accepted when that -- or sub_0800A95C's answer, where it has one -- is
 * positive. Everything else refuses.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `return 0;` must be the function's last statement and every `return 1;`
 *     must sit inside an arm. The compiler gives the value returned by the last
 *     statement the block that falls into the function's exit; the mirror image
 *     puts those two blocks the other way round and costs an extra branch.
 *   - sub_0800A6AC really is called a second time with the same arguments in
 *     the else arm. That is what the original does.
 */

int CanPlaceRiverAt(int x, int y)
{
    int v;
    int w;

    v = sub_0800A6AC(x, y);
    if (v < 0)
        return 0;
    if (v != 0)
    {
        w = sub_0800A95C(x, y);
        if (w == 0)
            return 1;
        if (w > 0)
        {
            if ((v & 0xFE00) == 0x4000)
                return 1;
            if ((v & 0xFE00) == 0x2000)
                return 1;
            if ((v & 0xFE00) == 0)
                return 1;
        }
    }
    else
    {
        if (sub_0800A6AC(x, y) < 0)
            return 0;
        v = sub_0800A884(x, y);
        if (v == 0)
            return 0;
        w = sub_0800A95C(x, y);
        if (w != 0)
            v = w;
        if (v > 0)
            return 1;
    }
    return 0;
}
asm(".global sub_0800AEAC\n.thumb_set sub_0800AEAC, CanPlaceRiverAt\n");
