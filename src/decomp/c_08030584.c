#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030584.
 * sub_08030584 @ 0x08030584, sub_08030600 @ 0x08030600, sub_08030670 @ 0x08030670, sub_080306E4 @ 0x080306E4, sub_08030768 @ 0x08030768
 */

void LinkStartKeySyncMode(void)
{
    struct Unk0849B018 *p;
    int i;

    p = gUnknown_0849B018;
    p->unk04 = 0;
    p->unk01 = 0;
    LinkResetKeySync();
    gUnknown_030040C0.unk06 = 0xa;
    gUnknown_030040C0.unk0a = 0;
    gUnknown_0300410C = gUnknown_030040CC;
    for (i = 0; i < 4; i++)
        gUnknown_03003F48[i] = gUnknown_03003128[i];
    gUnknown_0849B018->unk01 = 2;
    gUnknown_0849B018->unk04 = 6;
    StopAllMusic();
    PlayMusic(0x190);
}
asm(".global sub_08030584\n.thumb_set sub_08030584, LinkStartKeySyncMode\n");

void LinkStartDataModeTimer88(void)
{
    struct Unk0849B018 *p;
    int i;

    p = gUnknown_0849B018;
    p->unk04 = 0;
    p->unk01 = 0;
    SioResetBuffers();
    gUnknown_030040C0.unk0a = 0x88;
    gUnknown_030040C0.unk06 = 0x88;
    gUnknown_0300410C = gUnknown_030040CC;
    for (i = 0; i < 4; i++)
        gUnknown_03003F48[i] = gUnknown_03003128[i];
    gUnknown_0849B018->unk01 = 1;
    gUnknown_0849B018->unk04 = 6;
}
asm(".global sub_08030600\n.thumb_set sub_08030600, LinkStartDataModeTimer88\n");

void LinkStartDataModeTimer26(void)
{
    struct Unk0849B018 *p;
    int i;

    p = gUnknown_0849B018;
    p->unk04 = 0;
    p->unk01 = 0;
    SioResetBuffers();
    FadeOutMusicDefault();
    gUnknown_030040C0.unk0a = 0x26;
    gUnknown_030040C0.unk06 = 0x26;
    gUnknown_0300410C = gUnknown_030040CC;
    for (i = 0; i < 4; i++)
        gUnknown_03003F48[i] = gUnknown_03003128[i];
    gUnknown_0849B018->unk01 = 1;
    gUnknown_0849B018->unk04 = 6;
}
asm(".global sub_08030670\n.thumb_set sub_08030670, LinkStartDataModeTimer26\n");

void LinkStartBarrierMode(void)
{
    u16 local;
    int i;

    local = 0x7FFF;
    gUnknown_0849B018->unk01 = 3;
    LinkResetKeySync();
    gUnknown_030040C0.unk06 = 0xa;
    gUnknown_030040C0.unk0a = 0;
    SioSend16(&local, 1);
    gUnknown_0300410C = gUnknown_030040CC;
    for (i = 0; i < 4; i++)
        gUnknown_03003F48[i] = gUnknown_03003128[i];
    gUnknown_0849B01C->unk212 = 0;
}
asm(".global sub_080306E4\n.thumb_set sub_080306E4, LinkStartBarrierMode\n");

void LinkEnterIdleMode(void)
{
    u16 local;
    int i;

    local = 0x7FFF;
    gUnknown_0849B018->unk01 = 0;
    SioSend16(&local, 1);
    gUnknown_0300410C = gUnknown_030040CC;
    for (i = 0; i < 4; i++)
        gUnknown_03003F48[i] = gUnknown_03003128[i];
    gUnknown_03003F1C = gUnknown_030044C4 = gUnknown_030040AC = 0;
}
asm(".global sub_08030768\n.thumb_set sub_08030768, LinkEnterIdleMode\n");
