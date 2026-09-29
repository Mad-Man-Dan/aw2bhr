#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080373F0.
 * sub_080373F0 @ 0x080373F0
 */

/* Returns 0 when `a1` fails the current mode's check: with byte 0x32 of
 * gUnknown_03003FC0 set, when GetMapArmyCount(a1) differs from a2; otherwise
 * when a1 is in 0xB7..0xBF, or when byte 1 is 3 and IsMapArmyCountValid(a1) is 0.
 *
 * The three failure exits are one `return 0` reached by goto. Written as
 * three `return 0`s, cross-jumping keeps the last copy, but the ROM's
 * surviving block is the middle one (after the range test), so the label sits
 * there. `goto ok` on equality gives the ROM's `beq ok; b fail` pair. */
u8 IsMapSelectableForSlots(u16 a1, u16 a2)
{
    extern u8 gUnknown_03003FC0[];

    if (gUnknown_03003FC0[0x32]) {
        if (GetMapArmyCount(a1) == a2)
            goto ok;
        goto fail;
    } else {
        if ((u16)(a1 - 0xb7) <= 8) {
        fail:
            return 0;
        }
        if (gUnknown_03003FC0[1] == 3 && (u8)IsMapArmyCountValid(a1) == 0)
            goto fail;
    }
ok:
    return 1;
}
asm(".global sub_080373F0\n.thumb_set sub_080373F0, IsMapSelectableForSlots\n");
