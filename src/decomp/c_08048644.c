#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048644.
 * sub_08048644 @ 0x08048644
 */

void ShopScreen_DrawCursorSprites(u16 a1, u16 a2)
{
    int i;

    ApplyPaletteExt(&gUnknown_0823E550[DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10)],
                    0x2e0,
                    (0x10 - DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10)) * 2);

    ApplyPaletteExt(gUnknown_0823E550,
                    (u16)((0x10 - DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10)) * 2 + 0x2e0),
                    DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10) * 2);

    PutSprite(0, (a1 + 8) & 0x1ff, a2 & 0xff, gUnknown_08615C4E, 0x745d);

    if (gUnknown_084C30F8->unk836 > 2)
    {
        if (gUnknown_084C30F8->unk020 != 0)
            DrawOamObject(0x43, 0x38, 0x30, 0, 0);

        if (gUnknown_02028E1C[gUnknown_084C30F8->unk020 + 3] != 0xff)
            DrawOamObject(0x44, 0x38, 0x68, 0, 0);

        i = DivRem(Div((u16)gGameClock, 4), 0x10) * 2;
        ApplyPaletteExt((u16 *)((u8 *)gUnknown_08239F84 + i), 0x298, 2);
    }
}
asm(".global sub_08048644\n.thumb_set sub_08048644, ShopScreen_DrawCursorSprites\n");
