#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805878C.
 * sub_0805878C @ 0x0805878C
 */

/* Walks the stride-8 gUnknown_03003338 record list (terminated by unk00 == 0)
 * and picks, independently, the highest-unk02 record with bit 15 of unk00 SET
 * and the highest-unk02 record with it clear. The unflagged winner wins the
 * return; the flagged one is the fallback.
 *
 * Two facts settled the last 22 bytes, both of them statement order:
 *
 *  - `p = gUnknown_03003338;` is its OWN statement, before the four zero
 *    inits, and the loop is `for (; p->unk00 != 0; p++)`. Written as a
 *    for-init the pool `ldr` sinks below the `movs rN,#0` run; the ROM loads
 *    the pointer first.
 *  - The tail is `if (bestF != 0) return bestF; return 0;`, NOT
 *    `if (bestF == 0) return 0; return bestF;`. When the whole remainder of a
 *    function is a single `return`, agbcc lays the `if` body AFTER it and
 *    branches to it on the TRUE condition, so the ROM's `bne` reaching
 *    `adds r0,r3,#0` names `bestF != 0` as the source condition. The two
 *    spellings are the same instructions in the opposite order.
 *
 * The store is through bestF, not p (`strh r0,[r3]` where r3 was just set from
 * r1); the 0x8000 and 0xff00 constants in ip/r4 are LICM hoists, not source.
 *
 * MATCHED. */
struct Unk03003338 *AiPickBestAttackCandidate(void)
{
    struct Unk03003338 *p;
    struct Unk03003338 *best;
    struct Unk03003338 *bestF;
    int max;
    int maxF;

    p = gUnknown_03003338;
    best = 0;
    bestF = 0;
    maxF = 0;
    max = 0;

    for (; p->unk00 != 0; p++)
    {
        if (p->unk00 & 0x8000)
        {
            if (p->unk02 > maxF)
            {
                maxF = p->unk02;
                bestF = p;
                if ((p->unk00 & 0xff00) != 0xff00)
                    bestF->unk00 = p->unk00 & 0xff;
            }
        }
        else
        {
            if (p->unk02 > max)
            {
                max = p->unk02;
                best = p;
            }
        }
    }

    if (best != 0)
        return best;
    if (bestF != 0)
        return bestF;
    return 0;
}
asm(".global sub_0805878C\n.thumb_set sub_0805878C, AiPickBestAttackCandidate\n");
