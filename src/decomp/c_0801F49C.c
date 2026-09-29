#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F49C.
 * sub_0801F49C @ 0x0801F49C, sub_0801F4A4 @ 0x0801F4A4
 */

u8 *sub_0801F49C(void)
{
    return gUnknown_081268F8;
}

void InitMapFloodHandler(void)
{
    gUnknown_030013EC = sub_0801F4B4;
}
asm(".global sub_0801F4A4\n.thumb_set sub_0801F4A4, InitMapFloodHandler\n");
