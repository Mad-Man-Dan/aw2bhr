#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013034.
 * sub_08013034 @ 0x08013034
 */

/* Walks a 2-byte-token stream, then trims the trailing run of {0x81,0x40}
 * pairs (a full-width space in this encoding) by re-terminating the string.
 * The cursor is `u8`: both steps round-trip through `lsls #0x18; lsrs #0x18`,
 * and the final store folds the last `+ 2` into the strb displacement. */
void TrimTrailingFullWidthSpaces(u8 *p)
{
    u8 i;

    i = 0;
    while (p[i] != 0)
        i += 2;

    while (i != 0)
    {
        i -= 2;
        if (p[i] != 0x81)
            break;
        if (p[i + 1] != 0x40)
            break;
    }

    p[i + 2] = 0;
}
asm(".global sub_08013034\n.thumb_set sub_08013034, TrimTrailingFullWidthSpaces\n");
