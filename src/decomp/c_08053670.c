#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053670.
 * sub_08053670 @ 0x08053670
 */

/* Wave 51, W51-A. Closes the wave-34 park, and the park's diagnosis was wrong.
 *
 * The residual was recorded as a pure register-allocation accident -- "x -> r3,
 * y -> r2, &gUnknown_0300454C -> r0 in the ROM against x -> r2, y -> r0 here;
 * the pool address simply lands in whichever low register the second `ldrh` did
 * not take", with statement order and types declared not in question. It is not
 * an allocation at all: RunDeathBatch takes THREE u16 arguments and the wave-34
 * draft passed two. The third argument is `y`, which is already in r2 at the
 * call, so the ROM sets only r0 and r1 and the "unexplained" assignment of y to
 * r2 and x to r3 is forced by the calling convention. Adding the argument fixes
 * the allocation as a side effect; no reordering of the stores was needed.
 *
 * The lesson generalises: an argument that the code already holds in the right
 * register costs ZERO instructions at the call site, so a missing trailing
 * argument reads exactly like a register-allocation residual. Check the declared
 * arity before attributing a diff to allocation.
 *
 * SETTLED:
 *   - The parameter is u16 (PROMOTE_MODE `lsls #0x10; lsrs #0x10` at entry) and
 *     the record stride is 0x6c, so the subscript is gUnknown_02029808[a].
 *   - The +0 index is re-read at each of the three subscripts because the
 *     StartBattleHudHpCounter call kills it; the two fetches after the call share one
 *     reload. That is CSE being blocked, not three reads in the source.
 *   - The guard is `x + y <= 0xfe` on the SUM (`adds; cmp #0xfe; bgt`), not two
 *     separate bounds tests.
 */
void StepDeathTimelineBody(u16 a)
{
    u16 x;
    u16 y;

    if (gUnknown_03004508
        == gUnknown_02029808[a].unk1a[gUnknown_02029808[a].unk00])
    {
        StartBattleHudHpCounter(a);

        x = gUnknown_02029808[a].unk02[gUnknown_02029808[a].unk00];
        y = gUnknown_02029808[a].unk0e[gUnknown_02029808[a].unk00];

        gUnknown_0300454C[0] = x;
        gUnknown_0300454C[1] = y;

        if (x + y <= 0xfe)
            RunDeathBatch(a, x, y);
    }
}
asm(".global sub_08053670\n.thumb_set sub_08053670, StepDeathTimelineBody\n");
