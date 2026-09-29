#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B0B4.
 * sub_0804B0B4 @ 0x0804B0B4
 */

void NameEntry_Alloc(void)
{
    gUnknown_030044E0 = HeapMalloc(0x6c);
}
asm(".global sub_0804B0B4\n.thumb_set sub_0804B0B4, NameEntry_Alloc\n");
