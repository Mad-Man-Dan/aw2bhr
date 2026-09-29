#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070AF8.
 * sub_08070AF8 @ 0x08070AF8
 */

#include "hardware.h"

void SoundVSyncOn_rev01(void)
{
    struct SoundInfo * soundInfo = gUnknown_03007FF0;
    u32 ident = soundInfo->ident;

    if (ident == MPLAY_ID_NUMBER)
        return;

    REG_DMA1CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_32BIT | DMA_REPEAT;
    REG_DMA2CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_32BIT | DMA_REPEAT;

    soundInfo->pcmDmaCounter = 0;
    soundInfo->ident = ident - 10;
}
asm(".global sub_08070AF8\n.thumb_set sub_08070AF8, SoundVSyncOn_rev01\n");
