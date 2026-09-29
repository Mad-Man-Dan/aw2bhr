#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BB90.
 * sub_0803BB90 @ 0x0803BB90
 */

/* The `if`/`return` pair is load-bearing: `return gUnknown_0200C420.unk04 != 0;`
 * drops the `movs r0, #0` block entirely and comes out four bytes short. */
int MainMenu_BattleMapsUsability(void)
{
    if (gUnknown_0200C420.unk04 != 0)
        return 1;

    return 0;
}
asm(".global sub_0803BB90\n.thumb_set sub_0803BB90, MainMenu_BattleMapsUsability\n");
