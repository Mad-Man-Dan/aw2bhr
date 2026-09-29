#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080485C0.
 * sub_080485C0 @ 0x080485C0
 */

u8 ShopMessage_Always(void)
{
    return 1;
}
asm(".global sub_080485C0\n.thumb_set sub_080485C0, ShopMessage_Always\n");
