#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C7FC.
 * sub_0802C7FC @ 0x0802C7FC
 */

bool8 IntelMenu_TermsUsability(void)
{
    if (gUnknown_085C77A0[gPlaySt.mapID].unk08 != 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C7FC\n.thumb_set sub_0802C7FC, IntelMenu_TermsUsability\n");
