#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ACF0.
 * sub_0803ACF0 @ 0x0803ACF0
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

void DebugEdit_Init(struct Unk0803ACF0 *p)
{
    p->unk20 = IsCampaignCompletionFlagSet(0x65);
    p->unk22 = IsCampaignCompletionFlagSet(0x66);
    p->unk24 = IsCampaignCompletionFlagSet(0x68);
    p->unk26 = IsCampaignCompletionFlagSet(0x67);
    p->unk28 = gUnknown_0200C420.unk00;
    p->unk2a = gUnknown_0200C420.unk04;
    p->unk2c = 0;
    p->unk2e = (gUnknown_0200C420.unk09 + gUnknown_0200C420.unk0a
                + gUnknown_0200C420.unk0b) != 0;
    p->unk30 = gUnknown_0200CD0C;
}
asm(".global sub_0803ACF0\n.thumb_set sub_0803ACF0, DebugEdit_Init\n");
