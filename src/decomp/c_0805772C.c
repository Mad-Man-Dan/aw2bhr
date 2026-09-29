#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805772C.
 * sub_0805772C @ 0x0805772C
 */

/* Wave 52, W52-B.  MATCHED, 5 attempts.
 *
 * Draws a 7-tile digit strip from the gUnknown_085538B2 row table into a BG
 * map, forwards or mirrored on bit 0 of the row number.  Same shape as
 * c_080576D4.c, which is the exemplar for the `(gUnknown_08553A20 + i)->unk00
 * + pos->x + ((...unk01 + pos->y) << 5)` offset and for the
 * `gUnknown_08562124[i & 1] * 0x1000` palette nibble.
 *
 * THREE THINGS WERE MEASURED, not guessed:
 *  - `v` must be an `int` with an explicit `(u16)` cast at each use, NOT a
 *    `u16` local.  A u16 local folds (agbcc knows the `ldrh` already
 *    zero-extended it) and the ROM's `lsls r1,r0,#0x10` in the first block
 *    plus `lsrs r1,r1,#0x11` in the else block -- one zero-extend split across
 *    a basic-block boundary, its second half merged with the `>> 1` --
 *    disappears.  This is the wave-15 probe blind spot, live in a real
 *    function.  Worth 4 bytes and the whole head.
 *  - BOTH loops use the explicit pointer form `*(dst + off ± k)`.  The
 *    subscript form `dst[off ± k]` associates the giv init the other way
 *    (`plus(off * 2, dst)` instead of `plus(dst, off * 2)`).  Mixing the two
 *    forms across the arms is worse than either pure one: 94.6%, with the
 *    residual in whichever arm still used a subscript.
 *  - The `+ 0x400` in the mirrored arm is the BG-map h-flip bit; it is a
 *    plain constant and agbcc hoists it (`movs r0,#0x80; lsls r0,r0,#3`).
 */

struct Unk8057Pos
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};

void DrawHpGaugeStrip(u16 *dst, int i, struct Unk8057Pos *pos)
{
    u8 *src;
    int v;
    int par;
    int off;
    int k;

    par = i & 1;
    v = gUnknown_02029B78[par];

    if ((u16)v == 1)
        src = gUnknown_085538B9;
    else
        src = gUnknown_085538B2[(u16)v >> 1];

    off = (gUnknown_08553A20 + i)->unk00 + pos->x
        + (((gUnknown_08553A20 + i)->unk01 + pos->y) << 5);

    if (par != 0)
    {
        for (k = 6; k >= 0; k--)
            *(dst + off - k) = src[k] + gUnknown_08562124[par] * 0x1000 + 0x400;
    }
    else
    {
        for (k = 0; k <= 6; k++)
            *(dst + off + k) = src[k] + gUnknown_08562124[par] * 0x1000;
    }
}
asm(".global sub_0805772C\n.thumb_set sub_0805772C, DrawHpGaugeStrip\n");
