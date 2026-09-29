#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030178.
 * sub_08030178 @ 0x08030178, sub_080301E8 @ 0x080301E8
 */

#include "hardware.h"

void sub_08030178(void)
{
    int i;

    gUnknown_0849B01C->unk00++;
    gUnknown_0849B01C->unk00 &= 7;
    InitKeySt();
    REG_IME = 0;
    for (i = 0; i < 4; i++)
        gUnknown_0849B01C->unk08[0][i] |= 0xFFFF;
    gUnknown_0849B01C->unk06 = 0x5FFF;
    gUnknown_0849B01C->unk05 = 0;
    gUnknown_0849B01C->unk04 = 1;
    REG_IME = 1;
}

void sub_080301E8(void)
{
    int i;

    for (i = 0; i < 4; i++)
        gUnknown_0849B01C->unk208[i] = gUnknown_0849B01C->unk08[gUnknown_0849B01C->unk05][i];
    gUnknown_0849B01C->unk05++;
    gUnknown_0849B01C->unk05 &= 0x3F;
}
