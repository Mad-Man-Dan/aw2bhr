#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AF00.
 * sub_0803AF00 @ 0x0803AF00
 */

struct Unk0803ACF0
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u16 unk24;
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u16 unk28;
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
};

void sub_0803AF00(struct Unk0803ACF0 *p)
{
    sub_0803CBA0(0x65, p->unk20);
    sub_0803CBA0(0x66, p->unk22);
    sub_0803CBA0(0x68, p->unk24);
    sub_0803CBA0(0x67, p->unk26);
    gUnknown_0200C420.unk00 = p->unk28;
    if (p->unk2a < p->unk28)
        p->unk2a = p->unk28;
    gUnknown_0200C420.unk04 = p->unk2a;
    if (p->unk2c != 0)
        sub_0803C890();
    if (p->unk2e == 0)
    {
        gUnknown_0200C420.unk09 = 0;
        gUnknown_0200C420.unk0a = 0;
        gUnknown_0200C420.unk0b = 0;
    }
    WriteProfile();
}
