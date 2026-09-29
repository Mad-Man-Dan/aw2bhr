#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000DC0.
 * sub_08000DC0 @ 0x08000DC0
 */

/* Two unconditional calls, then a bit-13 gate on the flag word at +0 of
 * gActiveMap. 0x4000 is `movs #0x80; lsls #7`, agbcc's way of building a
 * single set bit above 255 without a pool word -- reading it as a shift of the
 * flag would be backwards, the shift is on the CONSTANT.
 *
 * gUnknown_03001FBC is declared s16 and arrives as a plain `ldrb` because
 * ClearSlotScriptCallback takes u8; the truncation is free in that direction. */
void DesignRoomProc_Loop(void)
{
    DesignRoomRunMode();
    DesignRoomDrawUi();

    if (gActiveMap->flags & 0x4000)
    {
        ClearSlotScriptCallback(gUnknown_03001FBC);
        gUnknown_030040A0 = 0;
    }
}
asm(".global sub_08000DC0\n.thumb_set sub_08000DC0, DesignRoomProc_Loop\n");
