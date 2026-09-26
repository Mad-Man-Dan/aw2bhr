#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018D70.
 * sub_08018D70 @ 0x08018D70
 */

bool8 sub_08018D70(s16 a)
{
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
