#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039140.
 * sub_08039140 @ 0x08039140
 */

/* An on-screen test for a box of size (w, h) at (x, y): the camera origin
 * lives at +0x04 / +0x06 of the map descriptor gUnknown_08499590 points at,
 * and the box is accepted when it overlaps the 0xF0 x 0xA0 screen.
 *
 * `cmn r1, r3` is the tell for the lower bounds: the comparison is against
 * MINUS the size, `dy > -h`, which agbcc folds into a compare-negative.
 * Writing it as `dy + h > 0` materialises the sum with an `adds` and costs 4
 * bytes.
 *
 * The first parameter arrives u16 (`lsls #0x10; lsrs #0x10`) and is cast back
 * to s16 at its single use (`lsls #0x10; asrs #0x10`); the second arrives
 * already s16. Both entry shift pairs are the declarations; only the one at
 * the use is a cast in the source.
 *
 * RETURN RETYPED FROM `int` TO `u8` in wave 38 (W38-F). The body cannot tell
 * the difference -- it returns the literals 1 and 0 -- but its only caller
 * sub_08039188 re-narrows the result with `lsls r0,#0x18` before testing it,
 * which is agbcc re-narrowing a u8-returning callee and is exactly what an
 * `int` return does not produce.
 *
 * DO NOT WIDEN PARAMETER 0. Wave 88 (W88-D) tested `int x` plus an explicit
 * `u16 a = (u16)x;` compensating cast -- the retype-plus-cast form that closed
 * BuildWarRoomCoSelectGroupList in the same wave -- to buy the 2 bytes the `u16` costs the only
 * caller. Result, by exit code: size-exact 72 bytes, 10 of 72 differ, and the
 * whole residual is a FOUR-INSTRUCTION BLOCK SWAP. A narrow PARAMETER's
 * conversion is emitted in the prologue insn group, ahead of every source
 * statement; the compensating cast IS a source statement, so it lands after
 * w's and h's conversions where the ROM has x's first. The cast reproduces the
 * conversion's instructions but never its position. `s16` is refuted too,
 * without a probe: a signed narrow parameter gets only ONE pair, the
 * sign-extending one (that is exactly what parameter y does here), and this
 * function needs both a zero-extend at entry and a re-sign at the use -- only
 * `u16` produces both. Full evidence in include/unknown-functions.h above the
 * declaration, and in work/sub_08039188/W88-notes.md. */

u8 IsRectOnScreen(x, y, w, h)
u16 x;
s16 y;
u8 w;
u8 h;
{
    int dx;
    int dy;

    dy = y - gMap->scrollY;

    if (dy > -h && dy <= 0x9f)
    {
        dx = (s16)x - gMap->scrollX;

        if (dx > -w && dx <= 0xef)
            return 1;
    }

    return 0;
}
asm(".global sub_08039140\n.thumb_set sub_08039140, IsRectOnScreen\n");
