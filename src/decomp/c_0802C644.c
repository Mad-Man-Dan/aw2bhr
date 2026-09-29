#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C644.
 * sub_0802C644 @ 0x0802C644
 */

/* Byte-for-byte the same function as IsLinkGame 0x18 bytes above it --
 * same global, same offset, same shape. A duplicate, not a family member. */
bool8 MapMenu_SaveUsability(void)
{
    if (gPlaySt.savingEnabled == 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C644\n.thumb_set sub_0802C644, MapMenu_SaveUsability\n");
