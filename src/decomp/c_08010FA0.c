#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010FA0.
 * sub_08010FA0 @ 0x08010FA0
 */

#include "hardware.h"

void InitFadeBlend(void)
{
    gUnknown_030030E0.raw = 0;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target1_enable_bg2 = 1;
    gUnknown_030030E0.bits.target1_enable_bg3 = 1;
    gUnknown_030030E0.bits.target1_enable_obj = 1;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;
}
asm(".global sub_08010FA0\n.thumb_set sub_08010FA0, InitFadeBlend\n");
