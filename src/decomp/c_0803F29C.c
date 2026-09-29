#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F29C.
 * sub_0803F29C @ 0x0803F29C
 */

/* Hands back a size pair for a mode id. Two separate `if`s, not `else if`:
 * the second `cmp r2, #4` is the fallthrough of the first block rather than
 * being branched over. Both out-params are words -- CannonFire_StartMuzzleEffect reads its two
 * stack slots back with `ldr`.
 */
void sub_0803F29C(int *a, int *b, int c)
{
    if (c == 3)
    {
        *a = 0x18;
        *b = 0x30;
    }

    if (c == 4)
    {
        *a = 8;
        *b = 0x10;
    }
}
