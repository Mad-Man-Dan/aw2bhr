#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043D5C.
 * sub_08043D5C @ 0x08043D5C
 */

void BuildUnlockedCoCarousel(void)
{
    RepeatUnlockedCoList();
    gUnknown_020288B0 = 0;
}
asm(".global sub_08043D5C\n.thumb_set sub_08043D5C, BuildUnlockedCoCarousel\n");
