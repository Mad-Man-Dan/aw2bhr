#include "global.h"
#include "hardware.h"

u16 sub_080303C8(void)
{
    int acc;
    int i;

    if (gPlaySt.savingEnabled == 0)
        goto reset;
    if (gUnknown_0849B018->unk01 != 2)
        goto reset;

    if (gUnknown_0849B018->unk06 != 0)
        while (gUnknown_02023894 == 0 && gUnknown_0849B018->unk04 != 2)
            ;

    REG_IME = 0;
    gUnknown_02023894 = 0;
    REG_IME = 1;
    sub_080301E8();

    gUnknown_0849B01C->unk06 = (~REG_KEYINPUT & 0x3FF)
                             | ((gUnknown_0849B01C->unk00 << 10) | 0x8000)
                             | (gUnknown_0849B01C->unk02 << 13);

    acc = 0;
    for (i = 0; i < 4; i++)
    {
        if (sub_0802F460(i))
        {
            if (gUnknown_0849B01C->unk208[i] == 0xFFFF
             || gUnknown_0849B01C->unk208[i] == 0x5FFF
             || gUnknown_0849B01C->unk208[i] == 0x7FFF
             || !(gUnknown_0849B01C->unk208[i] & 0x8000))
            {
                gUnknown_0849B01C->unk210 |= 0xFFFF;
                goto keys;
            }
            if (((gUnknown_0849B01C->unk208[i] & 0x1C00) >> 10) != gUnknown_0849B01C->unk00)
            {
                gUnknown_0849B018->unk1b++;
                gUnknown_0849B01C->unk210 |= 0xFFFF;
                return 0;
            }
            acc |= gUnknown_0849B01C->unk208[i] & 0x3FF;
        }
    }

    gUnknown_0849B01C->unk210 = acc;
    gUnknown_0849B018->unk1b = 0;
    return gUnknown_0849B01C->unk210;

reset:
    gUnknown_0849B018->unk1b = 0;
keys:
    return gpKeySt->previous;
}
