#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800EAF4.
 * sub_0800EAF4 @ 0x0800EAF4
 */

/* Retile a 2x2 block when the cell at (x, y) is one of the two halfword ids
 * 0x86 / 0x87.  The plane arithmetic is c_0800164C.c's sub_080016D0 read: the
 * `rowOffset[y] + x` index off gUnknown_08499590's +0x417A row table, doubled,
 * into the halfword plane at +0xA22.
 *
 * `subs #0x86; lsls #0x10; lsrs #0x10; cmp #1; bhi` is gcc's range test for
 * `v == 0x86 || v == 0x87`, and the truncation pair is the u16 width of `v`,
 * not a cast this function performs.
 *
 * NEITHER `x + 1` NOR `y + 1` is a mutation or a named local here, and the
 * register allocation is the only thing that says so.  Writing `x++` between
 * the second and third call gives byte-identical instructions but swaps r4 and
 * r5 between x and y+1: the increment's pseudo is created at the statement and
 * outranks the CSE temp for y + 1, where in the ROM x holds r4 from the
 * prologue and y + 1 gets r5.  Spelling both as plain expressions at their use
 * sites leaves x's own parameter pseudo as the higher-priority allocno, which
 * is the ROM's assignment.  Same for a `n = y + 1;` statement, whether it sits
 * before or after the first call. */
void MakeForestBlock2x2(int x, int y)
{
    u16 v;

    v = gMap->tile[gMap->rowOffset[y] + x];

    if (v == 0x86 || v == 0x87)
    {
        sub_0800EBFC(x, y, 0x25);
        sub_0800EBFC(x, y + 1, 0x65);
        MakeTileSimple(x + 1, y, 0x27);
        MakeTileSimple(x + 1, y + 1, 0x67);
    }
}
asm(".global sub_0800EAF4\n.thumb_set sub_0800EAF4, MakeForestBlock2x2\n");
