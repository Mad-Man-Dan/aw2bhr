#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802ECEC.
 * sub_0802ECEC @ 0x0802ECEC
 */

#include "hardware.h"

void sub_0802ECEC(int cycles)
{
    REG_TM3CNT_L = -cycles;
    REG_TM3CNT_H = TIMER_ENABLE | TIMER_IRQ | TIMER_PRESCALE_1024;
}
