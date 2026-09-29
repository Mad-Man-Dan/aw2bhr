#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C78C.
 * sub_0802C78C @ 0x0802C78C, sub_0802C7A0 @ 0x0802C7A0
 */

bool8 OptionsMenu_MusicOnUsability(void)
{
    if (gPlaySt.bgmOn == 1)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C78C\n.thumb_set sub_0802C78C, OptionsMenu_MusicOnUsability\n");

bool8 OptionsMenu_MusicOffUsability(void)
{
    if (gPlaySt.bgmOn == 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C7A0\n.thumb_set sub_0802C7A0, OptionsMenu_MusicOffUsability\n");
