#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080119A0.
 * sub_080119A0 @ 0x080119A0, sub_080119D4 @ 0x080119D4, sub_08011A20 @ 0x08011A20
 */

/* Draws a NUL-terminated string by handing each character to the glyph
 * dispatcher PutAsciiGlyphSprite and advancing x by one 8-pixel cell whenever the
 * dispatcher reports a full-height glyph (it returns 0 for the three glyphs it
 * draws seven pixels higher, and those do NOT advance the cursor).
 *
 * The `(u8)` on the result is real: PutAsciiGlyphSprite returns `int`, and an `int`
 * truth test compiles to a bare `cmp r0, #0`. The ROM's `lsls r0, r0, #0x18`
 * before the compare is the QImode truncation. */
void PutAsciiStringSprites(u16 x, u16 y, const char *s)
{
    while (*s)
    {
        if ((u8)PutAsciiGlyphSprite(x, y, *s++))
            x += 8;
    }
}
asm(".global sub_080119A0\n.thumb_set sub_080119A0, PutAsciiStringSprites\n");

/* Prints a value in HEX, least significant nibble first, walking x LEFTWARDS
 * one 8-pixel cell per digit. The digit tiles are 0x3D0..0x3D9 for 0-9 and
 * 0x3E0..0x3E5 for a-f, so the two arms are spelled differently in the source:
 * `d | 0x3D0` for the decimal digits (an OR that agbcc builds from
 * `movs #0xf4; lsls #2`) and `d + 0x3D6` for the letters (a pooled word).
 *
 * Neither call narrows its third argument even though PutGlyphSprite takes a
 * u16: combine can see that `(v & 0xF) | 0x3D0` and `(v & 0xF) + 0x3D6` both
 * fit 16 bits, so the zero-extension folds away. PutDecimalNumberSprites, whose digit
 * comes out of __umodsi3, keeps the shift pair for exactly that reason.
 *
 * The shifted-down remainder is bound to a SECOND local and it is that local,
 * not `v`, that the exit test reads. That is worth 2 bytes and it is the whole
 * difference between this and the obvious `v >>= 4; } while (v != 0);`:
 * testing `t` keeps `t` live across the `v = t` copy, so the two pseudos
 * conflict and the allocator cannot coalesce them -- hence the ROM's
 * `lsrs r0, r5, #4; adds r5, r0, #0` instead of an in-place `lsrs r5, r5, #4`.
 * Six other spellings (`v /= 16`, `while ((v >>= 4) != 0)`, a temp tested
 * through `v`, a `for(;;)`/`break`, a separate local seeded from the
 * parameter) all coalesce and all come out 2 bytes short. */
void PutHexNumberSprites(u16 x, u16 y, u32 v)
{
    u32 d;
    u32 t;

    do
    {
        d = v & 0xF;
        if (d <= 9)
            PutGlyphSprite(x, y, d | 0x3D0);
        else
            PutGlyphSprite(x, y, d + 0x3D6);
        x -= 8;
        t = v >> 4;
        v = t;
    } while (t != 0);
}
asm(".global sub_080119D4\n.thumb_set sub_080119D4, PutHexNumberSprites\n");

/* The decimal sibling of PutHexNumberSprites: prints `v` least significant digit
 * first, walking x leftwards one 8-pixel cell per digit. Tile 0x3D0 is '0'.
 *
 * The third parameter is `u32`, not `int` -- __udivsi3/__umodsi3 rather than
 * __divsi3/__modsi3 -- and it is the PARAMETER that is unsigned rather than a
 * local copy of it: `adds r4, r2, #0` is the first instruction after the push,
 * ahead of both u16 narrowings, which is where PROMOTE_MODE puts a parameter
 * save. A `u32 v = n;` statement inside the body sinks that copy below them. */
void PutDecimalNumberSprites(u16 x, u16 y, u32 v)
{
    do
    {
        PutGlyphSprite(x, y, v % 10 + 0x3D0);
        x -= 8;
        v /= 10;
    } while (v != 0);
}
asm(".global sub_08011A20\n.thumb_set sub_08011A20, PutDecimalNumberSprites\n");
