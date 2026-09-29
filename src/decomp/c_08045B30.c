#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045B30.
 * sub_08045B30 @ 0x08045B30
 */

/* MATCHED -- wave 45 (W45-A). Was parked at 83.85% (wave 35, W35-G) with the
 * whole instruction sequence already the ROM's and only REGISTER NUMBERING
 * left. Two changes closed it, and the second is the general result:
 *
 * 1. The FIRST row offset is not a variable. Writing rowOffset[6] inline at
 *    both of its uses makes it a CSE-created pseudo, which is what puts it in
 *    r1 (the scratch that just held the pool constant) instead of the
 *    callee-saved register a declared local gets.
 *
 * 2. THE OTHER THREE ARE THREE SEPARATE LOCALS, NOT ONE REUSED `a`. This is
 *    the whole difference between 83.85% and a match, and the mechanism is
 *    allocno priority in gcc's global_alloc:
 *
 *        priority ~ floor_log2(n_refs) * n_refs * freq / live_length
 *
 *    One `a` assigned three times is ONE pseudo with 9 refs spanning three
 *    quarters of the function: floor_log2(9) = 3, so priority ~ 3*9/45 = 0.6.
 *    That BEATS the terrain base (3*9/64 = 0.42) and the 0xe0 constant
 *    (3*10/62 = 0.48), so it is allocated first and takes r2, pushing the
 *    constant to r3 and terrain to r4 -- exactly the old draft's wrong
 *    allocation.
 *
 *    Split into b/c/d, each pseudo has 3 refs. floor_log2(3) COLLAPSES TO 1,
 *    so priority ~ 1*3/14 = 0.21 -- now the LOWEST of the five. The constant
 *    is allocated first (r2), then terrain (r3), and b/c/d, whose ranges do
 *    not overlap, all land in r4. That is the ROM.
 *
 * THE RULE, which is worth more than this function: a variable reused across
 * N independent statements is one high-priority pseudo, and N separate
 * variables are N low-priority ones. The cliff is at 4 REFS, where
 * floor_log2 steps from 1 to 2 -- so splitting a local is a large, DISCRETE
 * lever on which register everything else gets, not a stylistic choice. Read
 * it backwards as a diagnostic: if the ROM parks a value in a callee-saved
 * register where your draft uses a scratch (or vice versa), count refs before
 * touching anything else.
 *
 * Still true from W35-G: the first test needs the mask AND the shift
 * (`(x & 0xe0) >> 5`) -- combine folds the AND away but it was EXPANDED
 * first, and THUMB's andsi3 takes no immediate, so the constant pseudo
 * already exists in that block. Still ruled out from W35-A: `(x & 0xe0) ==
 * 0x20` as the first test (14.1%); `u16` for the row offset (byte-identical);
 * a flat `u8 *cells = p + 0x1432` local. 0x4186/0x4188/0x418E/0x4190 are
 * rowOffset[6]/[7]/[10]/[11]. */
int MapEventCond_Army1OwnsEightFixedCells(void)
{
    struct Map *m = gMap;
    int b;
    int c;
    int d;

    if (((m->terrain[m->rowOffset[6] + 8] & 0xe0) >> 5) != 1)
        return 0;
    if ((m->terrain[m->rowOffset[6] + 0xb] & 0xe0) != 0x20)
        return 0;

    b = m->rowOffset[7];
    if ((m->terrain[b + 7] & 0xe0) != 0x20)
        return 0;
    if ((m->terrain[b + 0xc] & 0xe0) != 0x20)
        return 0;

    c = m->rowOffset[10];
    if ((m->terrain[c + 7] & 0xe0) != 0x20)
        return 0;
    if ((m->terrain[c + 0xc] & 0xe0) != 0x20)
        return 0;

    d = m->rowOffset[11];
    if ((m->terrain[d + 8] & 0xe0) != 0x20)
        return 0;
    if ((m->terrain[d + 0xb] & 0xe0) != 0x20)
        return 0;

    return 1;
}
asm(".global sub_08045B30\n.thumb_set sub_08045B30, MapEventCond_Army1OwnsEightFixedCells\n");
