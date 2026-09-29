#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010F38.
 * sub_08010F38 @ 0x08010F38
 */

/* Writes the string `s` into the tilemap `dest` at cell (x, y) on a 32-tile
 * row, one tile per character: digits (<= 0x40) index the glyph run based at
 * gUnknown_0300308C[0], everything above 0x40 the letter run at
 * gUnknown_0300308C[1]. A space blanks the cell to tile 0 first -- the store is
 * then immediately overwritten by the general case below it, which is what the
 * ROM does and not an artefact to spell away.
 *
 * The `(u16)` cast on the base-minus-bias subexpression is load-bearing, and it
 * must sit on THAT subexpression rather than on the whole store: it is what
 * makes shorten_binary_op do the arithmetic in HImode, where -0x41 and -0x30
 * become the 16-bit constants 0xFFBF and 0xFFD0 that need pool words -- the
 * ROM's two `.4byte`s and the r5 they cost, hence `push {r4, r5, lr}`. Cast the
 * whole expression instead, or leave it off, and the sum of a u8 and a u16 can
 * overflow HImode so fold keeps it wide and folds each bias into a
 * `sub rN, #0x41` immediate, losing both pool words and the register.
 *
 * The index must be parenthesised as `dest + (x + y * 32)`: written
 * `dest + x + y * 32` agbcc scales and adds the two terms separately instead of
 * forming the cell index first and scaling once. */
void DrawAsciiStringToTilemap(u16 x, u16 y, const u8 *s, u16 *dest)
{
    u16 *p;

    p = dest + (x + y * 32);
    while (*s != 0)
    {
        if (*s == 0x20)
            *p = 0;
        if (*s > 0x40)
            *p = *s + (u16)(gUnknown_0300308C[1] - 0x41);
        else
            *p = *s + (u16)(gUnknown_0300308C[0] - 0x30);
        s++;
        p++;
    }
}
asm(".global sub_08010F38\n.thumb_set sub_08010F38, DrawAsciiStringToTilemap\n");
