#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C5F0.
 * sub_0802C5F0 @ 0x0802C5F0
 */

bool8 IntelMenu_RulesUsability(void)
{
    if (gPlaySt.gameMode == 3)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C5F0\n.thumb_set sub_0802C5F0, IntelMenu_RulesUsability\n");
