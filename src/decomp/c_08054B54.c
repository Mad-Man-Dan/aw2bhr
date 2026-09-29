#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08054B54.
 * sub_08054B54 @ 0x08054B54
 */

#include "hardware.h"

void BattleAnimEnableInterrupts(void)
{
    REG_IME = 1;
    REG_IE |= INTR_FLAG_VBLANK;
    REG_IE |= INTR_FLAG_TIMER0;
    REG_DISPSTAT = 8;
}
asm(".global sub_08054B54\n.thumb_set sub_08054B54, BattleAnimEnableInterrupts\n");
