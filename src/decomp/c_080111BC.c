#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080111BC.
 * sub_080111BC @ 0x080111BC
 */

#include "hardware.h"

void sub_080111BC(void)
{
    REG_DMA0CNT_H = 0;
}
