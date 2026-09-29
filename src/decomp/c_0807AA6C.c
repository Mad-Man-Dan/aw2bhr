#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807AA6C.
 * sub_0807AA6C @ 0x0807AA6C
 */

/* sub_0807A8F4's neighbour: same guard on the same predicate, but it plays a
 * sound instead of calling sub_0803B3C8. The two differ in the guarded call and
 * nothing else. */
void sub_0807AA6C(void)
{
    if (sub_08078E20() == 0)
        PlayMusic(0xCD);
}
