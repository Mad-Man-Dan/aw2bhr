#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030CCC.
 * sub_08030CCC @ 0x08030CCC
 */

void sub_08030CCC(void)
{
    u8 v;

    v = sub_08030D1C();
    if (v == 1)
    {
        gUnknown_0849B018->unk1e = 0;
        gUnknown_0849B018->unk04 = 4;
        gUnknown_0849B018->unk04 = 5;
        gPlaySt.savingEnabled = v;
        SioSend16((u16 *)&gGameClock, 1);
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
