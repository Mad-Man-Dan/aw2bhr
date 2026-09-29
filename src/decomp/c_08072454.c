#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072454.
 * sub_08072454 @ 0x08072454
 */

#include "hardware.h"

void Fade_WhiteCallBack(void)
{
    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x10;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;
}
asm(".global sub_08072454\n.thumb_set sub_08072454, Fade_WhiteCallBack\n");
