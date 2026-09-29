#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073974.
 * sub_08073974 @ 0x08073974
 */

/* SetCircleWindowEdge's two-plane twin: the same 0..0x9F row guard and the same
 * clamp, but the address is `base + 4 * y + 2 * c`, i.e. a halfword table with
 * TWO entries per row and `c` selecting which. `lsls #1; adds; lsls #1; adds`
 * is that index, not a stride-3 multiply.
 *
 * Two differences from the twin and both are real: the high clamp tests
 * `> 0xF0` rather than `> 0xEF`, and there is no `& 0xff` on the stored value. */

void SetWipeEdge(int x, u32 y, int c, u16 *base)
{
    if (y <= 0x9f)
    {
        if (x < 0)
            x = 0;

        if (x > 0xf0)
            x = 0xf0;

        base[y * 2 + c] = x;
    }
}
asm(".global sub_08073974\n.thumb_set sub_08073974, SetWipeEdge\n");
