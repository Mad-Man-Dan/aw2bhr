#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C31C.
 * sub_0803C31C @ 0x0803C31C, sub_0803C320 @ 0x0803C320
 */

int ReturnOne(void)
{
    return 1;
}
asm(".global sub_0803C31C\n.thumb_set sub_0803C31C, ReturnOne\n");

int ReturnZero2(void)
{
    return 0;
}
asm(".global sub_0803C320\n.thumb_set sub_0803C320, ReturnZero2\n");
