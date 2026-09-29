#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801FCE0.
 * sub_0801FCE0 @ 0x0801FCE0, sub_0801FD30 @ 0x0801FD30
 */

/* Paints a 3-wide vertical bar of a3 into the gUnknown_03003340 row-pointer
 * screen, from row a2 down to the bottom edge (the +0x02 height of the
 * gUnknown_08499590 map), starting at column a1.
 *
 * The row index IS the second parameter, walked in place: the `lsls #16;
 * lsrs #16` at the loop bottom is the u16 wrap of `a2++` and the guard/latch
 * pair is UNSIGNED (`bhs`/`blo`) because both operands are known
 * zero-extended halfwords. The prototype in include/unknown-functions.h stays
 * `(int, int, int)` -- the narrowing is done by three local copies at the top
 * of the body, which puts the `lsls/lsrs` triple in the prologue exactly as
 * declared narrow parameters would but WITHOUT forcing the truncation onto
 * c_0803E9F8.c's already-matched call site. Probed both spellings this wave;
 * the bodies are identical.
 *
 * The height bound is re-read from memory every iteration (c_0801F838's rule);
 * only the row address `&gUnknown_03003340[y]` is hoisted out of the inner
 * loop, which is the compiler's own CSE and not a source local. */
void MapSetBarToBottom(int a1, int a2, int a3)
{
    u16 x;
    u16 y;
    u8 v;
    int i;

    x = a1;
    y = a2;
    v = a3;
    for (; y < gMap->height; y++)
        for (i = 0; i < 3; i++)
            gUnknown_03003340[y][x + i] = v;
}
asm(".global sub_0801FCE0\n.thumb_set sub_0801FCE0, MapSetBarToBottom\n");

/* Draws a crosshair of a3 across the gUnknown_03003340 screen centred on
 * (a1, a2): the whole of row a2 except column a1, then the whole of column a1
 * except row a2. Both `!=` guards are the ROM's `beq` skips.
 *
 * Parameters are narrowed by three local copies rather than being declared
 * u16/u16/u8, which produces the same prologue `lsls/lsrs` triple while
 * leaving the `(int, int, int)` prototype -- and c_0803E9F8.c's matched call
 * site -- alone. Probed both spellings this wave; the bodies are identical.
 *
 * The two bounds are the map's width (+0x00) and height (+0x02), re-read from
 * memory at every loop bottom. Neither loop authors a pointer: the first
 * hoists `&gUnknown_03003340[a2]` as an invariant and the second's
 * `adds r1, #4` is strength_reduce turning the `gUnknown_03003340[i]`
 * subscript into a post-increment giv, with the giv's `ldr r1,=` init landing
 * in the preheader after the zero-trip guard. Writing either as an explicit
 * walking pointer emits the `ldr` ahead of the guard and reorders the pool. */
void MapSetCross(int a1, int a2, int a3)
{
    u16 x;
    u16 y;
    u8 v;
    int i;

    x = a1;
    y = a2;
    v = a3;
    for (i = 0; i < gMap->width; i++)
        if (i != x)
            gUnknown_03003340[y][i] = v;
    for (i = 0; i < gMap->height; i++)
        if (i != y)
            gUnknown_03003340[i][x] = v;
}
asm(".global sub_0801FD30\n.thumb_set sub_0801FD30, MapSetCross\n");
