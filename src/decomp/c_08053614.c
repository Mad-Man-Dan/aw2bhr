#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053614.
 * sub_08053614 @ 0x08053614
 */

/* Wave 51, W51-A. Closes the wave-34 park.
 *
 * struct UnkVec is two WORDS, so the field poke is a 32-bit `& ~0xc00` on unk04
 * and not a halfword mask: GetSlotSpriteAttrs returns the pair by value through the
 * hidden r0 pointer and SetSlotSpriteAttrs takes it back in r1/r2, both moving it as
 * `ldr [sp]` / `ldr [sp,#4]`.
 *
 * THE RESIDUAL WAS THE `orrs` OPERAND ORDER, and binding the shift to its own
 * statement is what sets the two halves independently. The ROM computes the
 * SHIFT first (`movs #3; ands; lsls #0xa` -> r3) but copies the MASKED value
 * into the destination (`adds r2,r0,#0; orrs r2,r3`), i.e. the mask is the `|`'s
 * LEFT operand while the shift is evaluated first. Written as a single
 * expression neither order gives both: `mask | sh` gets the operand roles right
 * and the evaluation order wrong, `sh | mask` the reverse. The wave-34 note
 * concluded no plain spelling sets them independently, which is right -- the
 * answer is that the shift is not part of the expression at all. Hoisting it to
 * a local fixes evaluation order (the local is dead by the `|`, so it costs no
 * register and does not disturb r5 holding `b` across the GetSlotSpriteAttrs call)
 * and leaves the mask as the left operand.
 *
 * SETTLED:
 *   - Parameters are (s16, u16). The `-1` guard is `a != -1` on the sign-
 *     extended r0 (`movs r0,#1; rsbs r0,r0,#0; cmp`), and `b` is zero-extended
 *     at entry by PROMOTE_MODE.
 *   - IsSlotScriptActiveAt returns a narrow value tested with a BARE `lsls #0x18` --
 *     a truth test, so the existing declaration is right and no re-narrowing
 *     belongs at the call site.
 *   - The mask is the 32-bit 0xFFFFF3FF, spelled `~0xc00`, and lands in the
 *     pool as one word.
 */
void SetSlotSpritePriority(s16 a, u16 b)
{
    struct UnkVec v;
    int s;

    if (a != -1 && IsSlotScriptActiveAt(a))
    {
        v = GetSlotSpriteAttrs(a);
        s = (b & 3) << 10;
        v.unk04 = (v.unk04 & ~0xc00) | s;
        SetSlotSpriteAttrs(a, v);
    }
}
asm(".global sub_08053614\n.thumb_set sub_08053614, SetSlotSpritePriority\n");
