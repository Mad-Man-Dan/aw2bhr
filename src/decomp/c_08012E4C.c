#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012E4C.
 * sub_08012E4C @ 0x08012E4C
 */

/* Buckets the low five bits of gGameClock into 0 / 1 / 2, with the top
 * bucket folding back to 1.
 *
 * TWO things here are forced by the codegen and neither is guessable from the
 * shape.
 *
 * The comparisons are UNSIGNED (`bhi` / `bls`) even though gGameClock
 * is a declared s32. Masking with 0x1F makes the value provably non-negative,
 * but agbcc will not use that: the local has to be spelled unsigned to get
 * them. An `int` local gives `bgt` / `ble` throughout and is a different
 * function.
 *
 * The SECOND local is what produces the ROM's `adds r0, r1, #0`, and it has to
 * be a copy of the first local rather than a second copy of the expression.
 * `w = gGameClock & 0x1F` written out again is folded back into one
 * pseudo by CSE and the copy disappears; `w = v` is a local-to-local copy that
 * survives, because it stays live across the first branch and gcc gives the
 * two pseudos different hard registers -- which is why the LAST comparison
 * reads r0 while the first two read r1. The copy is dead on the `movs r0, #0`
 * path, so it costs 2 bytes plus 2 more of pool realignment: exactly the 4
 * this function was short.
 *
 * What the second name MEANT in the original source is not recoverable -- two
 * differently-named variables for the same quantity produce this too. Only
 * that there were two is proved.
 *
 * Return type is byte-neutral (u32 and u8 emit the same bytes here); spelled
 * u32 to agree with the locals. */
u32 GetClockPhase(void)
{
    u32 v = gGameClock & 0x1F;
    u32 w = v;

    if (v <= 0xB)
        return 0;

    if (v <= 0xF)
        return 1;

    if (w <= 0x1B)
        return 2;

    return 1;
}
asm(".global sub_08012E4C\n.thumb_set sub_08012E4C, GetClockPhase\n");
