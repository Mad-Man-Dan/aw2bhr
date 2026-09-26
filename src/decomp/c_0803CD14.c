#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CD14.
 * sub_0803CD14 @ 0x0803CD14
 */

int sub_0803CD14(u8 id)
{
    return gUnknown_020280C0[id].unk13;
}
