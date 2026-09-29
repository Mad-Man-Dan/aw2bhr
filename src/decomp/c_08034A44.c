#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034A44.
 * sub_08034A44 @ 0x08034A44, sub_08034A58 @ 0x08034A58
 */

/* Centred x for a string on a 240-pixel line. The halving is a bare `lsrs #1`,
 * so the subtraction is UNSIGNED -- that is what fixes sub_0808B6B0's return
 * type as u32 rather than a narrow or signed one, since a signed `/ 2` would
 * have been the `lsr #31; add; asr #1` triple. */
int GetCenteredTextX(const char *s)
{
    return (0xf0 - sub_0808B6B0(s) * 8) / 2;
}
asm(".global sub_08034A44\n.thumb_set sub_08034A44, GetCenteredTextX\n");

/* Draw `s` centred on row `y`. Both shift pairs are the int -> u16 conversions
 * PutAsciiStringSprites's first two parameters force, not evidence of narrow returns or
 * narrow parameters here: `adds r4, r0, #0` with a bare prologue says `y`
 * arrives wide. */
void PutCenteredAsciiStringSprites(int y, const char *s)
{
    PutAsciiStringSprites(GetCenteredTextX(s), y, s);
}
asm(".global sub_08034A58\n.thumb_set sub_08034A58, PutCenteredAsciiStringSprites\n");
