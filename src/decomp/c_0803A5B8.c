#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A5B8.
 * sub_0803A5B8 @ 0x0803A5B8
 */

void UnitClassInfo_DrawSelectionBrackets(void)
{
    u8 d;
    u8 c;
    u8 b;
    u8 a;

    a = gUnknown_0849E318[gUnknown_0849D89C->unk09].unk00;
    b = gUnknown_0849E318[gUnknown_0849D89C->unk09].unk02;
    c = gUnknown_0849E358[gUnknown_0849D89C->unk09].unk00;
    d = gUnknown_0849E358[gUnknown_0849D89C->unk09].unk02;
    switch (gUnknown_0849D89C->unk09)
    {
    case 4:
        c = GetStringPixelWidthSimple(gTextTable[gUnknown_085D5ABC[gUnknown_0849D89C->unk04->type].unk02]);
        break;
    case 0xb:
        c = GetStringPixelWidthSimple(gTextTable[gUnknown_085D5ABC[gUnknown_0849D89C->unk04->type].unk04]);
        break;
    }
    DrawCornerBracketSprites(a + gUnknown_0849D89C->unk00, b, c, d);
}
asm(".global sub_0803A5B8\n.thumb_set sub_0803A5B8, UnitClassInfo_DrawSelectionBrackets\n");
