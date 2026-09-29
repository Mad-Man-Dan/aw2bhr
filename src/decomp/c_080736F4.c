#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080736F4.
 * sub_080736F4 @ 0x080736F4
 */

/* A scanline writer: clamp x into 0..0xF0 and drop it into row y of a halfword
 * table, ignoring rows past 0x9F. The row test is UNSIGNED (`bhi`) while both
 * clamps are signed (`bge`, `ble`), so y is u32 and x is int -- x has to be
 * signed for the `< 0` clamp to mean anything.
 *
 * The two clamp bounds are NOT the same constant: the high test is `> 0xEF`
 * but the value stored is 0xF0. Its 36-byte twin SetWipeEdge tests `> 0xF0`
 * instead, so this is a source difference and not an off-by-one to normalise
 * away. */

void SetCircleWindowEdge(int x, u32 y, u16 *row)
{
    if (y <= 0x9f)
    {
        if (x < 0)
            x = 0;

        if (x > 0xef)
            x = 0xf0;

        row[y] = x & 0xff;
    }
}
asm(".global sub_080736F4\n.thumb_set sub_080736F4, SetCircleWindowEdge\n");
