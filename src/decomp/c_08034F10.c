#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034F10.
 * sub_08034F10 @ 0x08034F10
 */

void SetMapStateResumeCursor(void)
{
    gUnknown_030032D8 = 20;
}
asm(".global sub_08034F10\n.thumb_set sub_08034F10, SetMapStateResumeCursor\n");
