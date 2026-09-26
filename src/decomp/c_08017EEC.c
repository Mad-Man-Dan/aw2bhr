#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017EEC.
 * sub_08017EEC @ 0x08017EEC
 */

void sub_08017EEC(void)
{
    gUnknown_030030A8 = gUnknown_03001420;
    gUnknown_03001FF4 = gUnknown_03001420 + 0x6f;
}
