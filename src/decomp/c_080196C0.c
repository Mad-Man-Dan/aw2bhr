#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080196C0.
 * sub_080196C0 @ 0x080196C0
 */

/* The index local has to be NARROW, and that is the whole difficulty here.
 * agbcc has two expansions for a comparison used as a value: the branching
 * `cmp r0,#0; beq .L; movs r0,#1` and the branchless `(x | -x) >> 31`
 * (`rsbs; orrs; lsrs #0x1f`) the ROM uses. They are the same six bytes, so the
 * size never gives it away. Which one appears is decided by the type of the
 * destination, not by the comparison: with `int i` or `u32 i` -- and equally
 * with the comparison written inline as the subscript, or as `?:`, or as `!!`,
 * or with the call hoisted into its own statement first -- every spelling
 * probes as the branching form. Assigning to a `u8` (or `u16`) local is what
 * produces the ROM's branchless form, because the narrowing makes gcc want the
 * value in a register rather than a jump. The same "narrow the destination and
 * the expansion changes" effect is what settles Menu_SlideCursorSprite's final shift.
 *
 * The separate statement is load-bearing for a second reason: written inline as
 * a subscript, the `gPlaySt.mapID` load and its `muls #0x5c` get
 * hoisted ABOVE the `bl` and the index degenerates into a conditional
 * `adds r4, #4`. The ROM evaluates the call first, so the index is its own
 * statement. */
void ApplyMapUnitTableForDifficulty(void)
{
    void *p;
    u8 i;

    i = IsHardCampaignMode() != 0;
    p = gUnknown_085C77A0[gPlaySt.mapID].unk34[i];
    if (p != NULL)
        ApplyUnitSpawnTable(p);
}
asm(".global sub_080196C0\n.thumb_set sub_080196C0, ApplyMapUnitTableForDifficulty\n");
