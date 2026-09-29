#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034DF8.
 * sub_08034DF8 @ 0x08034DF8
 */

void sub_08034DF8(void)
{
    if (sub_08019260())
        return;

    if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
        return;

    InitTextTileCache(0);

    if (gPlaySt.savingEnabled == 0
     || gPlayers[gUnknown_030033EC].aiControlled == 1)
    {
        ScrollCameraToKeepCellInView(gPlayers[gUnknown_030033EC].cursorX,
                     gPlayers[gUnknown_030033EC].cursorY);
    }

    if (FindSlotScript((s32)gUnknown_0849A00C) == -1)
    {
        PlayArmyCoMusic(gUnknown_030033EC);
        InitCursorInfoPanelPosition();
        StartPendingWeatherChange();
        gUnknown_030032D8 = 7;
    }
}
