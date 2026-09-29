#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019E50.
 * sub_08019E50 @ 0x08019E50
 */

/* `cmn r1, r0` compares the SUM against zero, so the test is on
 * `g[0] + g[1]`, not on either byte alone. */
void sub_08019E50(void)
{
    if (gUnknown_030033E8[0] + gUnknown_030033E8[1] == 0)
        CloseTopMenu();
}
