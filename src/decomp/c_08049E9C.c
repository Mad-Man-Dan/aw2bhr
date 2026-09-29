#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049E9C.
 * sub_08049E9C @ 0x08049E9C
 */

void DefeatFlow_StartBanner(void)
{
    PlayMusic(0xda);
    sub_080152EC(gUnknown_084C325C, 0);
}
asm(".global sub_08049E9C\n.thumb_set sub_08049E9C, DefeatFlow_StartBanner\n");
