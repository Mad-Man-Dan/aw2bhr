#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C214.
 * sub_0802C214 @ 0x0802C214
 */

void sub_0802C214(void)
{
    CopyBytes((u8 *)gUnknown_08499588, (u8 *)gBG0TilemapBuffer + 0x380, 0x180);
    CopyBytes((u8 *)gUnknown_0849958C, (u8 *)gBG2TilemapBuffer + 0x380, 0x180);
    sub_08013AD4(0);
    sub_08013AD4(2);
}
