#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080D30.
 * sub_08080D30 @ 0x08080D30
 */

#include "hardware.h"

/* SuperCoPowerBg_Init @ 0x08080D30, 204 bytes, THUMB. Matched.
 *
 * Straight-line display-shadow setup: same family as the
 * 0x0806717C/08067300/08067410/08067F5C/0806A4DC group noted next to
 * gUnknown_030030E0 in hardware.h -- a raw HImode read-modify-write sets
 * a whole target group, and the paired target*_enable_bd bit is cleared
 * right after through .bits, reusing the same clear-mask register across
 * both the byte-0 and byte-1 halves of the union.
 *
 * gUnknown_03001FFC = gUnknown_03001FFC; is a genuine self-store -- only
 * spellable because the global is volatile.
 */
void SuperCoPowerBg_Init(void)
{
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = gUnknown_03001FFC;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 1;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x800;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 3;
    gUnknown_0300251C.bits.priority = 1;

    SetDispEnable(1, 1, 1, 1, 1);
}
asm(".global sub_08080D30\n.thumb_set sub_08080D30, SuperCoPowerBg_Init\n");
