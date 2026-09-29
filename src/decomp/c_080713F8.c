#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080713F8.
 * sub_080713F8 @ 0x080713F8
 */

/*
 * MPlayTempoControl. MATCHES -- but only when compiled with `old_agbcc`.
 * It does NOT match under the default build flags, so `try_match` and a plain
 * `python tools/trymatch.py MPlayTempoControl` both still report 32 bytes and fail.
 *
 * Verified byte-for-byte (bytes and relocations) with:
 *
 *   CC1    = tools/agbcc/bin/old_agbcc
 *   CFLAGS = the Makefile's CFLAGS with `-fprologue-bugfix` removed
 *            (old_agbcc rejects that option outright:
 *             "old_agbcc: Invalid option `-fprologue-bugfix'")
 *
 * `-fforce-addr` is irrelevant here -- it matches with and without.
 * The compiler binary is the whole difference, not a flag: this same source
 * under the normal agbcc minus `-fprologue-bugfix` is 36 bytes and 57%.
 *
 * This is the m4a sound library, and the two binaries disagree about it the way
 * they disagree about m4a.c in every other GBA tree. What old_agbcc does that
 * agbcc does not:
 *
 *   - it keeps `mplayInfo->tempoU` live across the multiply instead of
 *     coalescing it with the parameter, which is what forces the
 *     `adds r0, r1, #0` copy, the r4 allocation and the `push {r4, lr}` frame
 *     -- i.e. exactly the "one extra live value" this draft was parked on;
 *   - it substitutes the just-stored register for the re-read of
 *     `mplayInfo->tempoU` rather than emitting a reload, which is what makes
 *     the pokeemerald-shaped `tempoD * tempoU` spelling legal here. Under
 *     agbcc that same spelling emits a second `ldrh` and was ruled out in an
 *     earlier wave for exactly that reason.
 *
 * The muls destination tie then follows from the second operand being the
 * re-read member rather than the parameter: `tempoD * tempoU` gives
 * `adds r0,r1,#0 ; muls r0,r4` (the ROM) and `tempoD * tempo` gives
 * `adds r0,r4,#0 ; muls r0,r1`. Six spellings probed under old_agbcc and only
 * the member re-read moves it -- `tempo * tempoD`, `tempoD * tempo`, a `u16`
 * local for tempoD in either operand order, `t = tempo; t *= tempoD`, and
 * `p = tempo; p *= tempoD` on an `s32` all give the wrong one.
 *
 * Promotion is blocked on a build-system decision, not on the C: the Makefile
 * needs a per-file `CC1`/`CFLAGS` override for whichever src/decomp file this
 * lands in. See "Per-file compiler and flags" in docs/agbcc-codegen.md. Every
 * already-promoted function between 0x0806F710 and 0x080745C0 compiles
 * identically under both binaries, so such an override costs nothing there.
 *
 * The trailing `mplayInfo->ident = ident` compiles to nothing and is what puts
 * `ident` in r3 rather than r1 -- see MPlayContinue. Keep it.
 */
void MPlayTempoControl(struct MusicPlayerInfo * mplayInfo, u16 tempo)
{
    u32 ident = mplayInfo->ident;

    if (ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->tempoU = tempo;
    mplayInfo->tempoI = (mplayInfo->tempoD * mplayInfo->tempoU) >> 8;
    mplayInfo->ident = ident;
}
asm(".global sub_080713F8\n.thumb_set sub_080713F8, MPlayTempoControl\n");
