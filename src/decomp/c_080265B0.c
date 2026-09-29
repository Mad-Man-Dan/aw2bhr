#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080265B0.
 * sub_080265B0 @ 0x080265B0
 */

/* Two calls with the same first argument and a different literal second, not
 * one call with a conditional value -- a `?:` would compute the constant into
 * a register and leave a single `bl`.
 *
 * a2 is narrow and never materialised: `lsls r1,r1,#0x18; cmp r1,#0` is a bare
 * shift used only to set the flags, which is the truth-test form
 * docs/agbcc-codegen.md records for a u8 tested against zero. a1 gets the full
 * `lsls; lsrs` because it is passed on. */

void AwardCapturePoints(u8 a1, u8 a2)
{
    if (a2 == 0)
        AddArmyBonusPointsNoOp(a1, 5);
    else
        AddArmyBonusPointsNoOp(a1, 0xA);
}
asm(".global sub_080265B0\n.thumb_set sub_080265B0, AwardCapturePoints\n");
