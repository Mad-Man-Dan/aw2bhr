#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012358.
 * sub_08012358 @ 0x08012358, sub_0801237C @ 0x0801237C
 */

#include "hardware.h"

void SetDefaultColorEffects(void)
{
    gUnknown_030030E0.raw = 0;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;
}
asm(".global sub_08012358\n.thumb_set sub_08012358, SetDefaultColorEffects\n");

void ResetWindowShadows(void)
{
    SetWinEnable(0, 0, 0);

    gUnknown_03002B40 = 0;
    gUnknown_03002B4C = 0;
    gUnknown_03002EFC = 0;
    gUnknown_03002B44 = 0;
    gUnknown_03002B68 = 0;
    gUnknown_030024E4 = 0;
    gUnknown_03002B30 = 0;
    gUnknown_030020B8 = 0;

    gUnknown_030030A4.raw = 0;
    gUnknown_030030DC.raw = 0;
}
asm(".global sub_0801237C\n.thumb_set sub_0801237C, ResetWindowShadows\n");
