#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E7C8.
 * sub_0802E7C8 @ 0x0802E7C8
 */

/* MATCHED, wave 60 (W60-G).  344 bytes, relocs match, byte-for-byte identical.
 *
 * The last residual was ONE source-level fact: the loop is a plain `while`,
 * not a hand-written `if (...) return 0;` guard followed by a `do { } while`.
 * W56-E and W60-C both had every instruction inside the body already correct
 * and were stuck at ~16% on what looked like a literal-pool-placement defect
 * plus a cross-jump.  Both symptoms came from the hand-written guard:
 *
 *   - gcc's own loop rotation duplicates the exit test into a guard, and the
 *     guard's branch to the `return 0` block lands out of `b<cond>.n` range, so
 *     it compiles to `bne` + long `b` -- which is what the ROM has.  A
 *     hand-written guard is an ordinary block: its test comes out
 *     byte-identical to the loop-bottom's, jump.c cross-jumps the two, and the
 *     function collapses by two instructions.
 *   - Those two instructions move the `return 1` and `return 0` blocks, which
 *     moves the barrier agbcc dumps the literal pool at, which is why the pool
 *     split came out 1 / 1 / 8-at-the-end instead of the ROM's
 *     1 / 1 / 8-in-the-middle.  One cause, three symptoms.
 *
 * Everything W56-E settled still holds and is what made the one-line change
 * land: nx/ny are `int` and x/y are `u16` (the tell is `mov ip, r3` sharing
 * `lsls r3, r7, #0x10` between `(s16)nx` and `x = nx`); the step is written in
 * the SHIFTED domain, `nx = (u32)(nx * 0x10000 + 0x10000) >> 16`, because the
 * natural `nx = (u16)(nx + 1)` lets cross-jumping merge the shift pair across
 * cases 0/1; the map read is gMap->unitUnk[gMap->rowOffset[y] + x]; the unk2a compare is c_08026F9C.c's idiom, where
 * element [n + 1] folds the 0x3c stride into the 0x2a member offset and emits
 * `adds r1, #0x66`; and `cur` must be a block-scope pointer declared at the top
 * of the loop BODY so its pseudo is created first and wins sl, leaving
 * gMap to be re-loaded from an inline pool word each iteration.
 * Hoisting `cur` above the loop inverts that choice and costs a -fforce-addr
 * .rodata address constant the ROM does not have.
 */
int TruncatePathAtHiddenEnemy(int a1, int a2, void *a3, int a4)
{
    s8 *p;
    u16 x;
    u16 y;
    int nx;
    int ny;
    u8 d;

    p = a3;
    d = a4;
    x = a1;
    y = a2;
    nx = x;
    ny = y;

    while (*p != 4 && *p != -1) {
        union Unk802C57CBuf *cur = &gUnknown_03003100;
        u8 tile;

        switch (*p) {
        case 0:
            nx = (u32)(nx * 0x10000 - 0x10000) >> 16;
            break;
        case 1:
            nx = (u32)(nx * 0x10000 + 0x10000) >> 16;
            break;
        case 2:
            ny = (u32)(ny * 0x10000 + 0x10000) >> 16;
            break;
        case 3:
            ny = (u32)(ny * 0x10000 - 0x10000) >> 16;
            break;
        }

        tile = gMap->unitUnk[gMap->rowOffset[(s16)ny] + (s16)nx];

        if (tile != 0
         && gPlayers[(tile >> 6) + 1].team != gPlayers[gUnknown_030033EC].team) {
            p[0] = 10;
            p[1] = d;
            cur->pos.unk00 = x;
            cur->pos.unk02 = y;
            gUnknown_03004074 = gUnknown_03003340[(s16)y][(s16)x];
            return 1;
        }

        x = nx;
        y = ny;
        p++;
    }

    return 0;
}
asm(".global sub_0802E7C8\n.thumb_set sub_0802E7C8, TruncatePathAtHiddenEnemy\n");
