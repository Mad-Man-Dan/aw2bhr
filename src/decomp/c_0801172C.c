#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801172C.
 * sub_0801172C @ 0x0801172C
 */

/* Glyph dispatcher: maps a printable ASCII code to a tile id and blits it.
 * Returns 1 normally and 0 for the three glyphs that are drawn seven pixels
 * higher -- both returns are real, the `movs r0, #0` tail is only reachable
 * from those three cases.
 *
 * Each case calls PutGlyphSprite itself rather than assigning a shared tile
 * variable. That is what keeps the four 0x3FD cases as four separate blocks
 * with four separate pool words: with a shared `t = 0x3FD; break;` the four
 * bodies are identical RTL and cross-jumping folds them into one, which the
 * ROM's jump table shows did not happen.
 *
 * The `(u8)` masks on the two range tests (`lsls #24; lsrs #24` around
 * `c - 'a'`) are the QImode arithmetic a u8 parameter forces; an int `c` would
 * compare the full word and emit no mask. */
int PutAsciiGlyphSprite(u16 x, u16 y, u8 c)
{
    switch (c)
    {
    case ' ':
        return 1;
    case ',':
    case '.':
        PutGlyphSprite(x, y, 0x3FC);
        return 1;
    case '-':
        PutGlyphSprite(x, y, 0x3FF);
        return 1;
    case '=':
        PutGlyphSprite(x, y, 0x3FD);
        return 1;
    case '_':
        PutGlyphSprite(x, y, 0x3FD);
        return 1;
    case '(':
    case '<':
        PutGlyphSprite(x, y, 0x3FA);
        return 1;
    case ')':
    case '>':
        PutGlyphSprite(x, y, 0x3FB);
        return 1;
    case ':':
        PutGlyphSprite(x, y, 0x3FD);
        return 1;
    case '/':
        PutGlyphSprite(x, y, 0x3FD);
        return 1;
    case '%':
        PutGlyphSprite(x, y, 0x3FE);
        return 1;
    case '?':
        PutGlyphSprite(x, y, 0x3DC);
        return 1;
    case '~':
        PutGlyphSprite(x, y - 7, 0x3DD);
        return 0;
    case '\'':
        PutGlyphSprite(x, y - 7, 0x3DE);
        return 0;
    case '^':
        PutGlyphSprite(x, y - 7, 0x3DF);
        return 0;
    default:
        if (c >= 'a' && c <= 'z')
            PutGlyphSprite(x, y, c + 0x37F);
        else if (c >= 'A' && c <= 'Z')
            PutGlyphSprite(x, y, c + 0x39F);
        else
            PutGlyphSprite(x, y, c + 0x3A0);
        return 1;
    }
}
asm(".global sub_0801172C\n.thumb_set sub_0801172C, PutAsciiGlyphSprite\n");
