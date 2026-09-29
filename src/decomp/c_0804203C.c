#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804203C.
 * sub_0804203C @ 0x0804203C
 */

/* The magic-number chain is a POINTER SUBTRACTION, not a division of an int:
 * `x * 0x55555555` negated is `x * 0xAAAAAAAB`, the modular inverse of 3, which
 * is how agbcc does the exact division by the 0x0c stride -- `neg; asr #2`. The
 * `>> 6` that turns the unit index into the army number then merges with that
 * `asr #2` into the ROM's single `asr #8`. It must be `>> 6` and NOT `/ 64`: a
 * signed division emits the round-toward-zero correction (`cmp #0; bge;
 * add #0x3f`) between the two shifts, which both blocks the merge and costs
 * four instructions.
 *
 * The 0x780 mask is the four-bit bitfield unk04_7. Testing a bitfield against
 * zero needs only the mask, not the usual extract shift pair.
 *
 * The two `return 0`s are written out separately and CROSS-JUMPED into one
 * block by agbcc -- that is what makes the first test's `beq` jump forward over
 * the second test to a label the second test also falls into. Folded into one
 * `if (A && B) return 1; return 0;` the polarity flips: `return 1` goes inline
 * and `return 0` lands after the pool, which is 4 bytes wrong. */
int IsIndirectFireUnitArmed(struct Unit *p)
{
    if (GetUnitFiringRangeWithCoBonus(((p - gUnits) >> 6) + 1, p->type) == 1)
        return 0;

    if (p->ammo != 0)
        return 1;

    return 0;
}
asm(".global sub_0804203C\n.thumb_set sub_0804203C, IsIndirectFireUnitArmed\n");
