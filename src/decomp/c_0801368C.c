#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801368C.
 * sub_0801368C @ 0x0801368C
 */

#include "hardware.h"

/* Uploads one palette run to BOTH the gPal shadow and palette RAM itself.
 *
 * `gPal + b / 2` and not `(u8 *)gPal + (b & 0xFFFE)`: they are the same
 * address, but the mask spelling costs a pool word for 0xfffe and an `ands`,
 * where the ROM has `lsrs r1, #0x11; lsls r1, #1` -- the u16 parameter's
 * entry narrowing (`>> 16`) merged with the `>> 1`, then the pointer scale
 * back up. That merge is only available when the halving is written as
 * arithmetic on a `u16 *`. The promoted ApplyPaletteExt next door does use the
 * mask form, but its `b` is `u32`, so it has no entry shift to merge with --
 * the two spellings are not interchangeable between them.
 *
 * The third parameter is u16 (entry `lsls #0x10; lsrs #0x10`), which is why
 * unknown-functions.h was corrected from the `int` this wave first guessed;
 * AnimateMapPreviewPalette, its only caller, passes a literal and was re-verified. */
void sub_0801368C(u16 *src, u16 b, u16 n)
{
    sub_08011C58(src, gPal + b / 2, n);
    sub_08011C58(src, (u8 *)PLTT + b, n);
}
