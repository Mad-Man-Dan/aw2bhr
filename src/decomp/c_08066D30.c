#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066D30.
 * sub_08066D30 @ 0x08066D30
 */

/* The twin of RulesScreenHighlightSelectedRuleOption over the other gUnknown_08580934 wrapper slot, and
 * it wants the same four things:
 *
 *   - `const` on the wrapper, so loop-invariant motion can hoist its load into
 *     the preheader while the RAM `->unk00` behind it still reloads every
 *     iteration;
 *   - the address taken into `pp`, so the symbol is named on only one side of
 *     the loop -- naming it on both routes it through a synthesised .rodata
 *     word under -fforce-addr and the pool relocation stops matching;
 *   - `i = 0` split out of the `for`, which is what keeps it ahead of the
 *     preheader's address load;
 *   - the `do { } while (0)` around the body, which is NOT decoration. Without
 *     it the cursor and the hoisted zero come out with r3 and r4 swapped, and
 *     that is the whole 4-byte difference. A plain nested block does not do it
 *     and neither does binding the element address instead; the degenerate loop
 *     is what moves the tie-break. Written the way the macro it plainly came
 *     from would expand.
 */
void MatchSetupHighlightSelectedRuleOption(void)
{
    struct Unk0816E1B8 *const *pp;
    struct Unk08580934 *cur;
    int i;

    i = 0;
    pp = &gUnknown_0816E164;

    for (; i <= 6; i++)
    {
        cur = gUnknown_0816E164->unk00;
        do { cur->unk54[i]->unk46 = 0; } while (0);
    }

    (*pp)->unk00->unk54[(*pp)->unk00->unk33]->unk46 = 1;
}
asm(".global sub_08066D30\n.thumb_set sub_08066D30, MatchSetupHighlightSelectedRuleOption\n");
