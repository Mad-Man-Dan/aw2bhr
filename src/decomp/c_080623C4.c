#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080623C4.
 * sub_080623C4 @ 0x080623C4
 */

/* AiPopNextInterestSeed @ 0x080623C4, 176 bytes, THUMB.  MATCHED (wave 48, W48-D).
 *
 * Walks the {u8 x; u8 y; s16 v;} list at gUnknown_03003F20 -- the same 4-byte
 * view of that buffer src/decomp/c_0804151C.c builds and terminates with a
 * 0xFFFF v -- and returns the entry with the smallest v whose map cell is
 * still empty, blanking every entry it rejects.
 *
 * Three things were load-bearing:
 *
 * 1. THE MAP ADDRESSING is now `gMap->rowOffset[y] + x` / `gMap->dangerMask[off]`
 *    (include/map.h) -- the struct member keeps 0x417A and 0x3262 on the base
 *    the same way c_0804151C.c's one-local-per-step idiom did, emitting
 *    `(map + K) + idx` rather than folding K into a load displacement. The
 *    plane here is +0x3262, not c_0804151C.c's +0x12.
 *
 * 2. `bestV` IS `s16`, NOT `int` AND NOT A CAST.  agbcc's PROMOTE_MODE keeps a
 *    sub-word local ZERO-extended in its register, so `bestV = q->v` on an s16
 *    member is a plain `ldrh` (only the low half is live) while every READ of
 *    bestV in int context is `lsls #0x10; asrs #0x10`.  Spelling it `int` with
 *    (s16) casts produces the same instructions here but is not what the
 *    original wrote; spelling it `int` WITHOUT casts loses the shift pairs.
 *
 * 3. THE TAIL NEEDS THE EXPLICIT `else`.  `if (v != 0x7fff) { body; return 1; }
 *    return 0;` and `if (v == 0x7fff) return 0; body; return 1;` BOTH put the
 *    short 0-arm inline and the body at the far label -- the ROM has it the
 *    other way round.  Only `if (v == 0x7fff) return 0; else { body; return 1; }`
 *    emits `beq` to a trailing `movs r0,#0` with the body falling through and
 *    the second 0x7FFF pool word between them.  Measured, not guessed. */

struct Unk623C4Cell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};

int AiPopNextInterestSeed(int *outX, int *outY)
{
    struct Unk623C4Cell *q;
    struct Unk623C4Cell *best;
    int off;
    s16 bestV;

    bestV = 0x7fff;
    q = (struct Unk623C4Cell *)gUnknown_03003F20;
    best = NULL;

    while (q->v != -1)
    {
        off = gMap->rowOffset[q->y] + q->x;
        if (gMap->dangerMask[off] != 0)
            q->v = 0x7fff;
        else if (q->v < bestV)
        {
            bestV = q->v;
            best = q;
        }
        q++;
    }

    if (bestV == 0x7fff)
        return 0;
    else
    {
        best->v = 0x7fff;
        *outX = best->x;
        *outY = best->y;
        return 1;
    }
}
asm(".global sub_080623C4\n.thumb_set sub_080623C4, AiPopNextInterestSeed\n");
