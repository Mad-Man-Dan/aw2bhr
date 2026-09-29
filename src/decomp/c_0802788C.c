#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802788C.
 * sub_0802788C @ 0x0802788C, sub_08027904 @ 0x08027904
 */

#include "hardware.h"

void DayStartScreen_BlendIn(void)
{
    gUnknown_03002020 = gUnknown_03001470[gUnknown_03001FBC].unk1e / 4;
    gUnknown_03002B28 = 0x10 - gUnknown_03001470[gUnknown_03001FBC].unk1e / 4;

    if (++gUnknown_03001470[gUnknown_03001FBC].unk1e > 0x20)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_0802788C\n.thumb_set sub_0802788C, DayStartScreen_BlendIn\n");

void DayStartScreen_BlendOut(void)
{
    gUnknown_03002020 = 8 - gUnknown_03001470[gUnknown_03001FBC].unk1e / 4;
    gUnknown_03002B28 = gUnknown_03001470[gUnknown_03001FBC].unk1e / 4 + 8;

    if (++gUnknown_03001470[gUnknown_03001FBC].unk1e > 0x20)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_08027904\n.thumb_set sub_08027904, DayStartScreen_BlendOut\n");
