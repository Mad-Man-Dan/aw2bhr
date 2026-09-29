#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A07C.
 * sub_0803A07C @ 0x0803A07C
 */

void UnitInfoPanel_DrawPictureAndFuel(void)
{
    struct Unit *u;
    void **tbl;
    int x;
    int n;
    s8 d;

    u = gUnknown_0849D89C->unk04;
    x = gUnknown_0849D89C->unk00 + 0x30;
    tbl = gUnknown_0849DC18;
    n = GetPlayerCoCountry(gUnknown_0849D89C->unk08) - 1;
    PutOamHi(x, 0x39, tbl[u->type * 15 + n], 0x32E8);
    DrawOamObject(gUnknown_0849E224[u->type], gUnknown_0849D89C->unk00 + 0x3a, 8, 0, 0);
    DrawOamObject(0x23, gUnknown_0849D89C->unk00 + 0x3a, 0x18, 0, 0);
    DrawOamObject(6, gUnknown_0849D89C->unk00 + 0x44, 0x28, 0, 0);
    d = (u->fuel <= 9) ? -4 : 0;
    DrawSpriteNumberFont1(d + gUnknown_0849D89C->unk00 + 0x58, 0x28, u->fuel);
    d = (gUnknown_085D5ABC[u->type].maxFuel <= 9) ? -4 : 0;
    DrawSpriteNumberFont1(d + gUnknown_0849D89C->unk00 + 0x64, 0x30, gUnknown_085D5ABC[u->type].maxFuel);
    PutOamHi(gUnknown_0849D89C->unk00 + 0x5b, 0x2c, gUnknown_0849D8A0, 0x13CA);
    sub_0803AB3C();
}
asm(".global sub_0803A07C\n.thumb_set sub_0803A07C, UnitInfoPanel_DrawPictureAndFuel\n");
