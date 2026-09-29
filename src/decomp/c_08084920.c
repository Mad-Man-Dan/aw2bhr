#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084920.
 * sub_08084920 @ 0x08084920
 */

/* `if (C) return FALSE; return TRUE;` in that order. Writing the condition
 * negated (`if (!AnyShopItemRemaining()) return TRUE; return FALSE;`) swaps the two
 * constants and turns the `bne` into a `beq`; see the block-ordering note added
 * to docs/agbcc-codegen.md this wave. */
bool8 IsShopSoldOut(void)
{
    if (AnyShopItemRemaining())
        return FALSE;
    return TRUE;
}
asm(".global sub_08084920\n.thumb_set sub_08084920, IsShopSoldOut\n");
