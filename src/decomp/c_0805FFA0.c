#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FFA0.
 * sub_0805FFA0 @ 0x0805FFA0
 */

void sub_0805FFA0(void)
{
    if (gUnknown_030046C0.unk00 != 1
     && gUnknown_030046C0.unk00 != 0xe
     && gUnknown_030046C0.unk00 != 0xd
     && gUnknown_030046C0.unk00 != 0xf
     && gUnknown_030046C0.unk00 != 0x10
     && gUnknown_030046C0.unk00 != 0x11
     && gUnknown_030046C0.unk00 != 0x12
     && gUnknown_030046C0.unk00 != 0x13)
    {
        gUnknown_030040D8->unk01 |= 1;
        gUnknown_030040D8->unk02 = gUnknown_03003100.pos.unk00;
        gUnknown_030040D8->unk03 = gUnknown_03003100.pos.unk02;
        RebuildMapUnitLayers();
    }

    switch (gUnknown_030046C0.unk00)
    {
    case 1:
        sub_080600F0();
        break;
    case 0xe:
        sub_08060110();
        break;
    case 0xd:
        sub_08060170();
        break;
    case 0xf:
        sub_080601C8();
        break;
    case 0x10:
        sub_080601DC();
        break;
    case 3:
        ApplyCaptureProgress();
    case 2:
    _redraw:
        CommitUnitMove();
        break;
    case 4:
        sub_080601F0();
        return;
    case 5:
        sub_08060264();
        return;
    case 7:
        LoadUnitIntoTransport();
        goto _redraw;
    case 8:
        sub_0806056C(1);
        sub_0806056C(0);
        gUnknown_030045D4 = 0xa;
        return;
    case 0xa:
        sub_08042998();
        goto _redraw;
    case 6:
        sub_08042B84();
        break;
    case 0xb:
        sub_08060684();
        goto _redraw;
    case 0xc:
        sub_080606A0();
        goto _redraw;
    case 0x12:
        sub_080606BC();
        break;
    case 0x13:
        sub_0802C16C();
        break;
    case 0x14:
        sub_080602C4();
        return;
    }

    gUnknown_03004780 = 2;
    gUnknown_030045D4 = 0;
}
