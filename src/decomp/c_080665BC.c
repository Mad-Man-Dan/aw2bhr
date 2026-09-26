#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080665BC.
 * sub_080665BC @ 0x080665BC
 */

void sub_080665BC(int index)
{
    gUnknown_08580934->unk74[index]->unk08 = 0;
}
