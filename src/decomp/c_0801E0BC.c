#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E0BC.
 * sub_0801E0BC @ 0x0801E0BC
 */

u16 GetHiOamCursor(void)
{
    return gUnknown_03002B54;
}
asm(".global sub_0801E0BC\n.thumb_set sub_0801E0BC, GetHiOamCursor\n");
