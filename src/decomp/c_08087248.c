#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087248.
 * sub_08087248 @ 0x08087248
 */

int GetMapPreviewScrollX(void)
{
    int y;

    if (gPlaySt.gameMode == 2)
    {
        y = (int)(gUnknown_03005918 * 4) >> 1;
        if (y + 0xab > 0xf0)
            return (u16)(gUnknown_03005918 * 4 - 0xf0);
        return (u16)(y - 0xab);
    }

    y = (int)(gUnknown_03005918 * 4) >> 1;
    if (y + 0xa3 > 0xf0)
        return (u16)(gUnknown_03005918 * 4 - 0xf0);
    return (u16)(y - 0xa3);
}
asm(".global sub_08087248\n.thumb_set sub_08087248, GetMapPreviewScrollX\n");
