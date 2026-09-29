#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080338FC.
 * sub_080338FC @ 0x080338FC, sub_08033910 @ 0x08033910
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* 0x17C as a literal: agbcc builds it `movs r1,#0xbe; lsls r1,#1`, the ROM's
 * two instructions. */
void LinkC3_Parallel_080338FD(void)
{
    LoadCoFullBodyAndPalette(0, 0x17C, 7);
}

/* FIVE arguments, the fifth on the stack. 0x717C needs a pool word, so it is
 * emitted first -- argument setup grouped by operand class again. */
void LinkC3_Parallel_IDLE_08033911(void)
{
    sub_08043C28(0x38, 0xB8, 0x717C, 0, 0);
}

asm(".global sub_080338FC\n.thumb_set sub_080338FC, LinkC3_Parallel_080338FD\n"
    ".global sub_08033910\n.thumb_set sub_08033910, LinkC3_Parallel_IDLE_08033911\n");
