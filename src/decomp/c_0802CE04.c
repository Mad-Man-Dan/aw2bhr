#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CE04.
 * sub_0802CE04 @ 0x0802CE04
 */

void OptionsMenu_ToggleMusic(void)
{
    gPlaySt.bgmOn = 1 - gPlaySt.bgmOn;

    switch (gPlaySt.bgmOn)
    {
    case 0:
        FadeOutMusicDefault();
        break;

    case 1:
        PlayArmyCoMusic(gUnknown_030033EC);
        break;
    }

    RebuildMenuItems();
    gUnknown_0200C420.unk14 = (gPlaySt.bgmOn == 0);
}
asm(".global sub_0802CE04\n.thumb_set sub_0802CE04, OptionsMenu_ToggleMusic\n");
