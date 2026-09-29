#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076F14.
 * sub_08076F14 @ 0x08076F14
 */

/* "Is (x, y) inside a radius-16 circle?" -- x*x + y*y against 0xff.
 * Both parameters carry `lsls #16; asrs #16`, so they are s16 objects and not
 * casts; an `int` parameter would have no extension at all.
 * The pool-free if/return pair is the long spelling: `return x*x + y*y <= 255;`
 * would keep the true block as a fallthrough and never materialise the 0. */

/* Wave 54 (W54-F): the RETURN type is `u8`, settled from the only caller.
 * SnapWorldMapCursorToMarker truth-tests the result with `lsls r0, r0, #0x18; cmp r0, #0`,
 * i.e. it re-narrows to a byte, which an `int` return would not produce. The
 * change is byte-neutral in this body (the two constants need no narrowing) and
 * fan-in is 1, so nothing else moves. */

u8 IsWorldMapMarkerNearCursor(s16 x, s16 y)
{
    if (x * x + y * y <= 0xff)
        return 1;

    return 0;
}
asm(".global sub_08076F14\n.thumb_set sub_08076F14, IsWorldMapMarkerNearCursor\n");
