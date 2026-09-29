#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071948.
 * sub_08071948 @ 0x08071948
 */

/*
 * MATCHES under the DEFAULT compiler. Wave 38 (W38-A).
 *
 * NOT m4a, despite sitting among m4a functions in code-0806CFC8.s: this is a
 * clipped BG tilemap blit. Its neighbour sub_08071918 is the parked
 * interworking-veneer case (label 8 bytes early, four leading `movs r0, r0`);
 * this is the next real function after it, not part of it.
 *
 * Also NOT an old_agbcc function, and that was measured rather than assumed:
 * under old_agbcc it grows to 128 bytes against 120, spills r9 and widens the
 * prologue. It is the counter-example that stops the m4a override being
 * generalised to the whole 0x0806F710-0x080745C0 window -- see the wave-38
 * chapter in docs/agbcc-codegen.md.
 *
 * Source layout: a 2-byte header (width-1, height-1) followed by halfword
 * tilemap entries. The header is read as ONE WORD and both fields taken out of
 * it -- `ldr r0,[r3]` for the shift-and-mask of the second byte, and combine
 * narrows the first byte's `& 0xff` back into a `ldrb` of the same address.
 * A `const u8 *` with `p[0]` / `p[1]` gives two `ldrb`s and does not match.
 *
 * `w` MUST be `u16`, not `u8`, and that one token was the whole difference
 * between 86.7% and the match -- same instructions, same order, same 120 bytes,
 * but the mode of the pseudo decides whether the 0xff constant can hoist above
 * the `ldr`. Found by the permuter (251 iterations); it changed exactly this
 * one word in the draft below. Written up in docs/agbcc-codegen.md.
 *
 * Two other things the diff pinned, both of which cost an attempt if guessed:
 *   - the inner loop is a `while`, not a `for`. The ROM decrements x BEFORE
 *     advancing s and d; `for (x = w; x >= 0; x--)` puts the decrement last.
 *   - the destination index is ONE expression scaled once:
 *     `dest + (x0 + (y0 + y) * 32)` gives `lsls #5; adds; lsls #1`, whereas
 *     `dest + (y0 + y) * 32 + x0` is two pointer additions and scales twice.
 *
 * The fifth parameter is `u16`, retyped from the `int` the header used to
 * declare -- the ROM normalises it `lsls #0x10; lsrs #0x10` straight out of
 * `[sp,#0x18]`, which is PROMOTE_MODE's prologue zero-extension and which an
 * `int` does not get. All four promoted callers pass the literal 0x8360 and
 * were re-verified byte-for-byte after the change.
 *
 * NOTE A BEHAVIOURAL QUIRK, reproduced deliberately and not a transcription
 * error: the inner clip test is `(u32)(x0 + x) <= 0x1f` on the DESCENDING
 * counter while the write pointer d ASCENDS, so the cell tested is not the cell
 * written. Likewise, when a row fails the vertical clip the whole inner loop is
 * skipped, so `s` is NOT advanced past that row's data and every later row
 * reads shifted source. Both are in the ROM; do not "fix" either.
 */
void TmApplyTsaClipped(u16 * dest, int x0, int y0, const void * src, u16 base)
{
    const u16 * s;
    u16 * d;
    s16 x, y;
    u16 w;

    s = (const u16 *)((const u8 *)src + 2);
    w = *(const u32 *)src & 0xff;
    y = (*(const u32 *)src >> 8) & 0xff;

    while (y >= 0)
    {
        if ((u32)(y0 + y) <= 0x1f)
        {
            d = dest + (x0 + (y0 + y) * 32);
            x = w;

            while (x >= 0)
            {
                if ((u32)(x0 + x) <= 0x1f)
                    *d = base + *s;

                x--;
                s++;
                d++;
            }
        }

        y--;
    }
}
asm(".global sub_08071948\n.thumb_set sub_08071948, TmApplyTsaClipped\n");
