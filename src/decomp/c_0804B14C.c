#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B14C.
 * sub_0804B14C @ 0x0804B14C
 */

void FreeNameEntry(void)
{
    sub_08014ED4(gUnknown_030044E0);
}
asm(".global sub_0804B14C\n.thumb_set sub_0804B14C, FreeNameEntry\n");
