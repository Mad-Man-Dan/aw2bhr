#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080707F4.
 * sub_080707F4 @ 0x080707F4
 */

#include "hardware.h"

/* SoundInit. The CpuSet source word is assigned at its USE, not at its
 * declaration: the ROM materialises one zero for `ident = 0` and keeps it in
 * r3 all the way to `str r3,[sp]` just before the call. Initialising it up
 * front emits the stack store in the prologue instead (measured, 54.0%). */
void SoundInit_rev01(struct SoundInfo *soundInfo)
{
    u32 zero;

    soundInfo->ident = 0;

    if (REG_DMA1CNT & 0x02000000)
        REG_DMA1CNT = 0x84400004;

    if (REG_DMA2CNT & 0x02000000)
        REG_DMA2CNT = 0x84400004;

    REG_DMA1CNT_H = 0x400;
    REG_DMA2CNT_H = 0x400;

    *(vu16 *)0x04000084 = 0x8f;
    *(vu16 *)0x04000082 = 0xa90e;
    *(vu8 *)0x04000089 = (*(vu8 *)0x04000089 & 0x3f) | 0x40;

    REG_DMA1SAD = (u32)soundInfo->pcmBuffer[0];
    REG_DMA1DAD = 0x040000a0;
    REG_DMA2SAD = (u32)soundInfo->pcmBuffer[1];
    REG_DMA2DAD = 0x040000a4;

    gUnknown_03007FF0 = soundInfo;

    zero = 0;
    CpuSet(&zero, soundInfo, 0x050003ec);

    soundInfo->unk06 = 8;
    soundInfo->unk07 = 0xf;
    soundInfo->unk38 = sub_080700C0;
    soundInfo->unk28 = DummyFunc_rev;
    soundInfo->unk2c = (void (*)(u8))DummyFunc_rev;
    soundInfo->unk30 = DummyFunc_rev;
    soundInfo->unk3c = DummyFunc_rev;

    sub_0806FBD4(&gUnknown_03005740);
    soundInfo->unk34 = &gUnknown_03005740;

    SampFreqSet_rev01(0x40000);

    soundInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_080707F4\n.thumb_set sub_080707F4, SoundInit_rev01\n");
