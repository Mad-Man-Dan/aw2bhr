#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ABDC.
 * sub_0803ABDC @ 0x0803ABDC
 */

#include "hardware.h"
struct Unk0803ABDC
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

void DebugFlagControl_Loop(struct Unk0803ABDC *p)
{
    int i;

    i = IsCampaignCompletionFlagSet(p->unk1e);
    if (i > 0)
        i = 1;
    PutAsciiStringSprites(0, 0, gUnknown_08090F94);
    PutAsciiStringSprites(0, 8, gUnknown_08090FA4);
    DrawSpriteNumberFont2(0x28, 8, p->unk1e);
    PutAsciiStringSprites(0x38, 8, gUnknown_0849E5F8[i]);
    if ((gpKeySt->pressed & 3) != 0)
    {
        WriteProfile();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
    else if ((gpKeySt->pressed & DPAD_LEFT) != 0)
        SetCampaignFlagBank1(p->unk1e, 0);
    else if ((gpKeySt->pressed & DPAD_RIGHT) != 0)
        SetCampaignFlagBank1(p->unk1e, 1);
    else
    {
        if ((gpKeySt->repeated & DPAD_UP) != 0 && p->unk1e > 0x20)
            p->unk1e--;
        if ((gpKeySt->repeated & DPAD_DOWN) != 0 && p->unk1e <= 0x5e)
            p->unk1e++;
    }
}
asm(".global sub_0803ABDC\n.thumb_set sub_0803ABDC, DebugFlagControl_Loop\n");
