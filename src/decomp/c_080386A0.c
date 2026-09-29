#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080386A0.
 * sub_080386A0 @ 0x080386A0, sub_080386B4 @ 0x080386B4, sub_080386C8 @ 0x080386C8
 */

int IsCampaignMode(void)
{
    if (gPlaySt.gameMode == 1)
        return 1;

    return 0;
}
asm(".global sub_080386A0\n.thumb_set sub_080386A0, IsCampaignMode\n");

int sub_080386B4(void)
{
    if (gPlaySt.gameMode == 2)
        return 1;

    return 0;
}

int IsVersusMode(void)
{
    if (gPlaySt.gameMode == 3)
        return 1;

    return 0;
}
asm(".global sub_080386C8\n.thumb_set sub_080386C8, IsVersusMode\n");
