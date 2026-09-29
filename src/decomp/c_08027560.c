#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027560.
 * sub_08027560 @ 0x08027560
 */

/* The ROM keeps BOTH arms of the `if` even though they differ only in the `+ 1`
 * -- `adds r1, r2, #0` is hoisted above the compare as the shared `a3`, and
 * only the increment is duplicated. `cmp r0, #0x7f; ble` is SIGNED, which is
 * what makes the first parameter `int` rather than a byte. a2 is dead. */
void LoadPopupIcon(int a1, int a2, int a3, struct Unk0801C210 *a4)
{
    if (a1 > 0x7f)
        AP_SwitchAnimation(a4, a3 + 1);
    else
        AP_SwitchAnimation(a4, a3);

    Decompress(gUnknown_081121D0, (void *)0x06013940);
    ApplyPaletteExt(gUnknown_081126E4, 0x260, 0x20);
}
asm(".global sub_08027560\n.thumb_set sub_08027560, LoadPopupIcon\n");
