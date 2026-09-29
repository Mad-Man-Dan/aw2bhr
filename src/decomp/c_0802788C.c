#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802788C.
 * sub_0802788C @ 0x0802788C, sub_08027904 @ 0x08027904
 */

#include "hardware.h"

void sub_0802788C(void)
{
    gUnknown_03002020 = gUnknown_03001470[gUnknown_03001FBC].unk1e / 4;
    gUnknown_03002B28 = 0x10 - gUnknown_03001470[gUnknown_03001FBC].unk1e / 4;

    if (++gUnknown_03001470[gUnknown_03001FBC].unk1e > 0x20)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}

void sub_08027904(void)
{
    gUnknown_03002020 = 8 - gUnknown_03001470[gUnknown_03001FBC].unk1e / 4;
    gUnknown_03002B28 = gUnknown_03001470[gUnknown_03001FBC].unk1e / 4 + 8;

    if (++gUnknown_03001470[gUnknown_03001FBC].unk1e > 0x20)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
