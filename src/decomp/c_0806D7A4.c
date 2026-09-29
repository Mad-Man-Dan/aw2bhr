#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D7A4.
 * sub_0806D7A4 @ 0x0806D7A4
 */

struct Unk6D7A4
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ u32 unk24;
    /* 0x28 */ int unk28;
    /* 0x2c */ u8 filler_2c[0x08];
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ const u16 *unk3c;
};

void sub_0806D7A4(struct Unk6D7A4 *p)
{
    p->unk34 += p->unk38;
    p->unk24 += p->unk34;

    if (p->unk3c == gUnknown_08581F12)
        DrawOamObject(0x8c, p->unk24 & 0x1ff, p->unk28 & 0xff, 0, 1);
    else
        DrawOamObject(0x8d, p->unk24 & 0x1ff, p->unk28 & 0xff, 0, 1);

    if (p->unk24 + 0x30 > 0x120)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
