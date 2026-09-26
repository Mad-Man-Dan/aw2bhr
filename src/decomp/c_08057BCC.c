#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057BCC.
 * sub_08057BCC @ 0x08057BCC
 */

void sub_08057BCC(int i)
{
    gUnknown_030005E8[i] = 1;
}
