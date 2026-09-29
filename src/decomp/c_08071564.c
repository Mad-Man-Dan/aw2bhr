#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071564.
 * sub_08071564 @ 0x08071564
 */

/*
 * ClearModM, the compiled copy. Its hand-written twin is sub_08070300 (track
 * in r1, both stores in the opposite order), recorded in data/asm-resident.json.
 *
 * MATCHES -- but only when compiled with `old_agbcc`. It does NOT match under
 * the default build flags, so `try_match` and a plain
 * `python tools/trymatch.py MP_clear_modM` both still report 28 bytes and fail.
 *
 * Verified byte-for-byte (bytes and relocations) with:
 *
 *   CC1    = tools/agbcc/bin/old_agbcc
 *   CFLAGS = the Makefile's CFLAGS with `-fprologue-bugfix` removed
 *            (old_agbcc rejects that option outright)
 *
 * `-fforce-addr` is irrelevant here -- it matches with and without. The
 * compiler binary is the whole difference: this source under the normal agbcc
 * minus `-fprologue-bugfix` is 36 bytes and 6%.
 *
 * What changed. The eighteen spellings probed in earlier waves were all probed
 * under agbcc, where a plain `if`/`else` statement gets rewritten into "store
 * the else value, conditionally overwrite" -- one arm, no `b`, one instruction
 * shorter than the ROM. old_agbcc does not do that rewrite: the same
 * `if`/`else` keeps both arms and the unconditional `b`, reads `track->flags`
 * AFTER the merge, and emits the ROM's dead `movs r2, #0` alongside the live
 * `movs r0, #0`. Every part of the parked diff was that one missing rewrite.
 *
 * The `?:` spelling this draft used to carry still hoists the flags load above
 * the cmp under old_agbcc, so the discriminator between the two shapes survives
 * the compiler change -- it is the statement form, and this is the statement
 * form.
 *
 * Promotion is blocked on a build-system decision, not on the C -- the Makefile
 * needs a per-file `CC1`/`CFLAGS` override. See "Per-file compiler and flags"
 * in docs/agbcc-codegen.md.
 */
void MP_clear_modM(struct MusicPlayerTrack * track)
{
    track->lfoSpeedC = 0;
    track->modM = 0;

    if (track->modT == 0)
        track->flags |= MPT_FLG_PITCHG;
    else
        track->flags |= MPT_FLG_VOLCHG;
}
asm(".global sub_08071564\n.thumb_set sub_08071564, MP_clear_modM\n");
