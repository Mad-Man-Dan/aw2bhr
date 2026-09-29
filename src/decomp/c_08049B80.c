#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049B80.
 * sub_08049B80 @ 0x08049B80
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* gUnknown_084C30F8 is a POINTER VARIABLE in ROM (`ldr rN,=sym; ldr rN,[rN]`),
 * and +0x836 / +0x837 are past `ldrb`'s imm5, which is why each address is
 * built with its own `adds`. The ROM shares one pool word for the constant and
 * bumps it (`ldr r2,=0x836; adds r1,r0,r2; adds r2,#1; adds r0,r0,r2`), which
 * is CSE on the displacement rather than anything in the source. */
void ShopScreen_UpdateMainMenuLock(void)
{
    if (gUnknown_084C30F8->unk836 == gUnknown_084C30F8->unk837)
        UnlockMainMenu();
    else
        LockMainMenu();
}

asm(".global sub_08049B80\n.thumb_set sub_08049B80, ShopScreen_UpdateMainMenuLock\n");
