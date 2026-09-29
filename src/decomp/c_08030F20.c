#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030F20.
 * sub_08030F20 @ 0x08030F20
 */

#include "hardware.h"

/* The same blend setup as src/decomp/c_08048E0C.c's tail, with BG2 as the first
 * target instead of BG1.
 *
 * Four bitfield writes come out as TWO read-modify-writes because agbcc merges
 * consecutive stores that land in the same byte, and the merge preserves source
 * order inside each byte -- which is how the operand order is recoverable at
 * all. Byte 0 gets `|0x40` (effect) then `|4` (target1 BG2); byte 1 gets
 * `|0x10` (target2 OBJ) then `|8` (target2 BG3), so OBJ is written BEFORE BG3
 * in the source even though its bit is higher. Writing them the other way round
 * swaps the two `movs`. */
void LinkScreenSetupBlend(void)
{
    SetDefaultColorEffects();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_03002020 = 0xF;
    gUnknown_03002B28 = 6;
}
asm(".global sub_08030F20\n.thumb_set sub_08030F20, LinkScreenSetupBlend\n");
