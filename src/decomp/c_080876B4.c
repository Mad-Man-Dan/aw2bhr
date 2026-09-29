#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080876B4.
 * sub_080876B4 @ 0x080876B4
 */

#include "hardware.h"

/* Sets up the map-view window layout. Sibling of SetDefaultDisplayState, which resets
 * the same shadows. The three branches differ only in which of the four
 * scroll/extent bytes they load and whether win0 is enabled. */
void MapSelectList_SetupWindows(void)
{
    SetDispEnable(1, 1, 1, 1, 1);

    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 1;
    gUnknown_030030A4.bits.win1_enable_bg2 = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 1;
    gUnknown_030030A4.bits.win1_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;

    if (gPlaySt.gameMode == 2)
    {
        gUnknown_03002B68 = 0;
        gUnknown_03002B30 = 0x28;
        gUnknown_030024E4 = 0x58;
        gUnknown_030020B8 = gUnknown_03005928 * 16 + 0x28;

        SetWinEnable(0, 1, 0);
    }
    else if (gpKeySt->pressed & (DPAD_RIGHT | DPAD_LEFT | R_BUTTON | L_BUTTON))
    {
        gUnknown_03002B68 = 0;
        gUnknown_03002B30 = 0;
        gUnknown_030024E4 = 0x48;
        gUnknown_030020B8 = gUnknown_03005928 * 16 + 0x48;

        SetWinEnable(0, 1, 0);
    }
    else
    {
        gUnknown_030030A4.bits.win0_enable_bg0 = 1;
        gUnknown_030030A4.bits.win0_enable_bg1 = 1;
        gUnknown_030030A4.bits.win0_enable_bg2 = 1;
        gUnknown_030030A4.bits.win0_enable_bg3 = 1;
        gUnknown_030030A4.bits.win0_enable_obj = 1;

        gUnknown_03002B40 = 0;
        gUnknown_03002EFC = 0x28;
        gUnknown_03002B4C = 0x48;
        gUnknown_03002B44 = 0x38;
        gUnknown_03002B68 = 0;
        gUnknown_03002B30 = 0x48;
        gUnknown_030024E4 = 0x48;
        gUnknown_030020B8 = gUnknown_03005928 * 16 + 0x48;

        SetWinEnable(1, 1, 0);
    }
}
asm(".global sub_080876B4\n.thumb_set sub_080876B4, MapSelectList_SetupWindows\n");
