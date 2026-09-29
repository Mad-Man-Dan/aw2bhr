#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060894.
 * sub_08060894 @ 0x08060894
 */

/* The AI's action-2 roll: it works the running counter up into a percentage of
 * two different denominators, gates on the difficulty table's three byte
 * thresholds, and finally picks action 2 or 1 on a random draw against the
 * caller's percentage.
 *
 * `gUnknown_0816DAD4` in the disassembly is agbcc's -fforce-addr address
 * constant for gUnknown_030046D4 (baserom.gba has 0x030046D4 in that word), so
 * the counter is named directly and the apparent double indirection is that
 * constant plus the volatile read. Every one of the four reads is its own
 * `ldr`, which is the volatile behaviour and not a missing CSE.
 *
 * The final compare is `bhs`, i.e. UNSIGNED, because GetNextRandomNumber returns `u32`
 * and the usual arithmetic conversions carry that through the `% 100` and into
 * the comparison against the sign-extended `s16` parameter. */
void AiConsiderBuildingFootUnit(s16 a)
{
    int r;
    int n;

    r = 0;

    if (!CountBuildablePropertiesOfKind(2))
        return;

    if (gUnknown_03004674 != 0)
        r = gUnknown_030046D4 * 100 / gUnknown_03004674;

    if (gUnknown_030046D4 >= gUnknown_085766E0->unk00
        && r > gUnknown_085766E0->unk02)
    {
        n = GetArmyFacilityCount(0);

        if (n == 0)
            return;

        r = gUnknown_030046D4 * 100 / n;

        if (r > gUnknown_085766E0->unk01)
            return;
    }

    if (GetNextRandomNumber() % 100 < a)
        gUnknown_030046C0.unk06 = 2;
    else
        gUnknown_030046C0.unk06 = 1;
}
asm(".global sub_08060894\n.thumb_set sub_08060894, AiConsiderBuildingFootUnit\n");
