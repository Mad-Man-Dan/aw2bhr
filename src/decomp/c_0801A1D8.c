#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A1D8.
 * sub_0801A1D8 @ 0x0801A1D8, sub_0801A240 @ 0x0801A240, sub_0801A2E4 @ 0x0801A2E4
 */

/* One horizontal strip of a window frame: the left cap 0x361, `width - 2`
 * middles 0x362, then the right cap 0x363, each OR'd with the palette.
 *
 * The palette arrives as `lsls #0x1c; lsrs #0x10` -- that is the u16
 * truncation of `pal << 12`, not a mask plus a shift.
 *
 * The `cmp r4,#0x1f / bhi` guard is INSIDE the loop body, not a loop bound:
 * the column and the destination advance even when the store is skipped.
 * It compares UNSIGNED, hence the (u32) cast on an `int` parameter. */
void DrawWindowFrameTopRow(u16 *dst, int col, s16 width, int pal)
{
    u16 i;
    u16 p = pal << 12;

    if ((u32)col < 0x20)
        *dst = 0x361 | p;
    dst++;
    col++;

    for (i = 0; i < width - 2; i++)
    {
        if ((u32)col < 0x20)
            *dst = 0x362 | p;
        dst++;
        col++;
    }

    if ((u32)col < 0x20)
        *dst = 0x363 | p;
}
asm(".global sub_0801A1D8\n.thumb_set sub_0801A1D8, DrawWindowFrameTopRow\n");

/* The middle row of a window frame, and the only one of the three strips that
 * writes FOUR cells: a left cap, a run of `width - 3` middles, then two tail
 * cells. Every cell comes from a two-entry u16 table selected by `kind`
 * (`lsls #0x10; asrs #0xf` is `tbl[(s16)kind]` on a u16 array, i.e. index*2,
 * not a shift). The middle table's element address is LICM-hoisted out of the
 * loop -- that is the optimiser, not source.
 *
 * The palette is the fifth argument, arriving at [sp,#0x14]. */
void DrawWindowFrameMiddleRow(u16 *dst, int col, s16 width, s16 kind, int pal)
{
    u16 i;
    u16 p = pal << 12;

    if ((u32)col < 0x20)
        *dst = gUnknown_0848A46C[kind] | p;
    dst++;
    col++;

    for (i = 0; i < width - 3; i++)
    {
        if ((u32)col < 0x20)
            *dst = gUnknown_0848A470[kind] | p;
        dst++;
        col++;
    }

    if ((u32)col < 0x20)
        *dst = gUnknown_0848A474[kind] | p;
    dst++;
    col++;

    if ((u32)col < 0x20)
        *dst = gUnknown_0848A478[kind] | p;
}
asm(".global sub_0801A240\n.thumb_set sub_0801A240, DrawWindowFrameMiddleRow\n");

/* The bottom row of a window frame. The two alternating middle tiles are not
 * literals: they are memcpy'd out of the 4-byte gUnknown_0808E5C0 template
 * onto the stack first (`sub sp,#4` is the u16[2]), then indexed `i & 1`.
 *
 * The palette local is assigned AFTER the memcpy call, which is where the ROM
 * computes it. `movs r7,#1` in the preheader is the mask parked in a register
 * by the loop optimiser -- not source. */
void DrawWindowFrameBottomRow(u16 *dst, int col, s16 width, int pal)
{
    u16 buf[2];
    u16 i;
    u16 p;

    sub_0808B6E8(buf, gUnknown_0808E5C0, 4);
    p = pal << 12;

    if ((u32)col < 0x20)
        *dst = 0x367 | p;
    dst++;
    col++;

    for (i = 0; i < width - 2; i++)
    {
        if ((u32)col < 0x20)
            *dst = buf[i & 1] | p;
        dst++;
        col++;
    }

    if ((u32)col < 0x20)
        *dst = 0x369 | p;
}
asm(".global sub_0801A2E4\n.thumb_set sub_0801A2E4, DrawWindowFrameBottomRow\n");
