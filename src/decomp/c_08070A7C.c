#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070A7C.
 * sub_08070A7C @ 0x08070A7C
 */

#include "hardware.h"

/* SoundVSyncOff: disarm the two PCM DMAs, offset ident by +10 to record it,
 * and zero both PCM buffers. SoundVSyncOn_rev01 (SoundVSyncOn, already matched) is
 * the exact inverse and subtracts the 10 again. */
void SoundVSyncOff_rev01(void)
{
    struct SoundInfo *soundInfo = gUnknown_03007FF0;
    u32 ident = soundInfo->ident;
    u32 zero;

    if (ident - MPLAY_ID_NUMBER > 1)
        return;

    soundInfo->ident = ident + 10;

    if (REG_DMA1CNT & 0x02000000)
        REG_DMA1CNT = 0x84400004;

    if (REG_DMA2CNT & 0x02000000)
        REG_DMA2CNT = 0x84400004;

    REG_DMA1CNT_H = 0x400;
    REG_DMA2CNT_H = 0x400;

    zero = 0;
    CpuSet(&zero, soundInfo->pcmBuffer, 0x05000318);
}
asm(".global sub_08070A7C\n.thumb_set sub_08070A7C, SoundVSyncOff_rev01\n");
