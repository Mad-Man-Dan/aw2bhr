#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066D74.
 * sub_08066D74 @ 0x08066D74
 */

#include "hardware.h"

void sub_08066D74(void)
{
    int sc;

    gUnknown_08580934->unk2a++;

    HandleRulesMenuInput();
    sub_08066C70(gUnknown_08580934->unk54[gUnknown_08580934->unk33]);
    sub_08066B8C(gUnknown_08580934->unk33);
    sub_08066D30();

    sc = (SinDegrees(gUnknown_08580934->unk2a * 16 % 360) >> 9) + 0x100;

    SetObjAffine(0,
                 Div(gSinLut[0x40] * 16, sc != 0 ? sc : 2),
                 Div(-gSinLut[0] * 16, sc != 0 ? sc : 2),
                 Div(gSinLut[0] * 16, sc != 0 ? sc : 2),
                 Div(gSinLut[0x40] * 16, sc != 0 ? sc : 2));

    if (gpKeySt->pressed & 1)
    {
        sub_0806377C(gUnknown_08580DD8);
        LockMainMenu();
    }
    else if (gpKeySt->pressed & 2)
    {
        gUnknown_08580934->unk31 = 1;
        sub_080733B8();
        sub_08064B68(2);
        gUnknown_08580934->unk30 = 1;
        sub_0803B4DC(0x66);

        if (gUnknown_08580934->unk08 == 2)
        {
            gUnknown_08580934->unk26 = 0;
            sub_0806540C();
        }
        else
        {
            sub_0806530C();
            gUnknown_08580934->unk26 = 1;
        }
    }
}
