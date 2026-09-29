#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026588.
 * sub_08026588 @ 0x08026588
 */

/* unk16 is a running count and unk18 its high-water mark. The two are sibling
 * members at constant offsets, so the `strh` does not kill the cached deref
 * and the plain repeated spelling gets one index computation. The `cmp; bls`
 * is unsigned, which is what types both halfwords.
 *
 * Wave 34 (W34-D) widened the signature to three parameters. `b` and `c` are
 * genuinely unused here -- the body below is unchanged and still byte-exact --
 * but WriteBackBattleResult sets r0, r1 and r2 before every `bl`, and computes r0 and r1
 * with a full pointer-difference-by-12 divide each. See the note in
 * include/unknown-functions.h. */
void RecordUnitDestroyed(u8 a, u8 b, u8 c)
{
    gPlayers[a].destroyedThisTurn++;

    if (gPlayers[a].destroyedThisTurn > gPlayers[a].totalDestroyed)
        gPlayers[a].totalDestroyed = gPlayers[a].destroyedThisTurn;
}
asm(".global sub_08026588\n.thumb_set sub_08026588, RecordUnitDestroyed\n");
