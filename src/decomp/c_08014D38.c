#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014D38.
 * sub_08014D38 @ 0x08014D38
 */

/* Measures a string in pixels: gUnknown_084C36E4[c] per character plus one
 * pixel of spacing before every character after the first, with 9 and 0xa
 * treated as two-byte escapes worth a fixed 16 pixels. GetStringWidthInTiles converts
 * the result to tiles.
 *
 * The 9..0xa test MUST be a `switch`, not `if (c >= 9 && c <= 0xa)`. The
 * `&&` folds to `(u8)(c - 9) <= 1` (measured: `subs #9; lsls #0x18; lsrs
 * #0x18; cmp #1; bhi`), where the ROM keeps two signed compares against the
 * bounds -- expand_case's range node. The fold also clobbers the character in
 * r0, which is why the `&&` spelling reloads it for the table index and the
 * ROM does not.
 *
 * The accumulator is a full `int` here, unlike GetStringPixelWidthSimple's u16: nothing
 * truncates it. */
int GetStringPixelWidth(const char *s)
{
    int w;
    int seen;

    w = 0;
    seen = 0;

    while (*s != 0)
    {
        switch (*s)
        {
        case 9:
        case 0xa:
            w += 0x10;
            s += 2;
            break;
        default:
            if (seen != 0)
                w++;

            w += gUnknown_084C36E4[*s];
            s++;
            seen = 1;
            break;
        }
    }

    return w;
}
asm(".global sub_08014D38\n.thumb_set sub_08014D38, GetStringPixelWidth\n");
