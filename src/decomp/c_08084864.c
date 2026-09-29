#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084864.
 * sub_08084864 @ 0x08084864, sub_0808488C @ 0x0808488C
 */

/* One shape instantiated twice with GetMainMenuTileHighlightPalette: IsMainMenuTileComplete(i) selects
 * between a per-index 32-byte record and one fixed record. Two things in here
 * are not free:
 *   - The `!= 0` with an explicit `else` is load-bearing. `if (x == 0) return
 *     <array>; return <fixed>;` compiles with the arms SWAPPED (`beq` to the
 *     array, fixed falling through) and does not match; the ROM branches away
 *     on non-zero and falls through into the array arm.
 *   - The pointer arithmetic must be written on a flat array, not as
 *     gUnknown_0823DC38[i] on a `u8 [][32]`. Both spell the same address here,
 *     but the sibling's `+ 6` only survives in the flat form -- see the
 *     comment on the global and docs/agbcc-codegen.md, wave 20 (W20-B). */

u16 *GetMainMenuTilePalette(int i)
{
    if (IsMainMenuTileComplete(i) != 0)
        return gUnknown_0812596C;
    else
        return gUnknown_0823DC38 + i * 16;
}
asm(".global sub_08084864\n.thumb_set sub_08084864, GetMainMenuTilePalette\n");

/* GetMainMenuTilePalette's shape with the index biased by 6 and the other fixed record.
 * The bias must stay INSIDE the address expression on a flat array: as
 * `gUnknown_0823DC38[i + 6]` on a `u8 [][32]` fold distributes the element-size
 * multiply and folds 6 * 32 into the symbol, so the ROM's runtime
 * `adds r0, r4, #6` disappears; and hoisting it into its own statement
 * (`j = i + 6;`) emits that add BEFORE the pool `ldr` instead of after it. */

u16 *GetMainMenuTileHighlightPalette(int i)
{
    if (IsMainMenuTileComplete(i) != 0)
        return gUnknown_0812598C;
    else
        return gUnknown_0823DC38 + (i + 6) * 16;
}
asm(".global sub_0808488C\n.thumb_set sub_0808488C, GetMainMenuTileHighlightPalette\n");
