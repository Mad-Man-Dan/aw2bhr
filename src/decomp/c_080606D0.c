#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080606D0.
 * sub_080606D0 @ 0x080606D0, sub_08060718 @ 0x08060718
 */

/* AiProduceUnits @ 0x080606D0, 72 bytes.
 *
 * The AI turn's outer driver: seed a difficulty budget, ask AiBuildPropertyList how
 * many decision passes this turn gets, then run AiBuildOneUnit that many times
 * with the budget.
 *
 * The frame is the whole job. `sub sp, #4; movs r1, #0; mov r0, sp;
 * strh r1, [r0]` is a plain `s16 v = 0;` -- and the r0 that `mov r0, sp` set up
 * for the `strh` is STILL LIVE at the `bl`, so AiCalcMechChance receives the
 * local's ADDRESS. That is the only evidence for its prototype, and it agrees
 * with the promoted definition in src/decomp/c_08060D4C.c, which writes
 * through the pointer on both arms.
 *
 * `movs r1, #0; ldrsh r0, [r5, r1]` inside the loop is agbcc's only spelling of
 * `ldrsh` (it has no immediate form), and it is INSIDE the loop because
 * AiCalcMechChance may have changed the local and nothing here caches it -- do not
 * hoist the read into a variable.
 *
 * The loop is a plain counted `for`, not a decrement of the count itself:
 * `cmp r4, #0; ble` guards it and the bottom is `subs r4, #1; cmp r4, #0;
 * bne`, which is check_dbra_loop reversing an ascending counter it knows runs
 * to zero. Spelling it `while (n-- > 0)` gives `bgt` and an extra `adds r0,
 * r4, #0` per iteration. */
void AiProduceUnits(void)
{
    s16 v;
    int n;
    int j;

    v = 0;
    AiCalcMechChance(&v);
    n = AiBuildPropertyList();
    gUnknown_030045D8 = AiCountEnemyUnitsOfType(0x15);

    for (j = 0; j < n; j++)
        AiBuildOneUnit(v);

    gUnknown_03004780 = 4;
}
asm(".global sub_080606D0\n.thumb_set sub_080606D0, AiProduceUnits\n");

/* AiBuildOneUnit @ 0x08060718, 208 bytes.
 *
 * One AI decision pass: clear the chosen-action state, retally the army's 64
 * unit slots into the two counters the scorers divide by, then run the scoring
 * chain and commit if anything was chosen.
 *
 * Three things here are agbcc's and not the source's:
 *
 *  - `gUnknown_03004674 = gUnknown_030045D0 = 0;` is a CHAIN, and the chain is
 *    what orders the literal pool. Written as two statements in either order,
 *    the pool word of whichever is assigned first comes first AND its store
 *    comes first; the ROM loads gUnknown_03004674's address first but stores
 *    gUnknown_030045D0 first, which only the chained form produces --
 *    expand_assignment takes the outer destination's address before descending
 *    into the inner assignment.
 *
 *  - the loop bound is bound to `end` and the START is the loop variable
 *    itself, with no third local. The ROM's `ldrh r3, [r0]` writes the
 *    induction register directly; a separate `start` local costs an extra
 *    `adds r3, r0, #0` copy. `subs r3, r4, r3` in the preheader and the
 *    `subs; cmp; bne` bottom are check_dbra_loop's, not a descending source
 *    loop.
 *
 *  - the table base is hoisted for the first read (`adds r2, r0, r1`) and
 *    RE-LOADED from gUnits for the second (`ldr r0, [r7]`), and both
 *    accumulators keep their addresses in registers across the loop
 *    (gUnknown_03004674 in r6, gUnknown_030045D0 through ip then r5). That
 *    asymmetry is LICM's; both accesses are written the same way below.
 *
 * The parameter arrives zero-extended (PROMOTE_MODE does that whatever the
 * signedness) and is re-signed with `lsls #0x10; asrs #0x10` before
 * AiConsiderBuildingFootUnit, whose promoted definition takes s16. */
void AiBuildOneUnit(s16 a1)
{
    int k;
    int end;

    gUnknown_030046C0.unk06 = 0;
    gUnknown_03004674 = gUnknown_030045D0 = 0;

    k = gUnknown_03003F2C;
    end = k + 0x40;

    for (; k < end; k++)
    {
        if (gUnits[k].type != 0)
            gUnknown_03004674++;

        if (gUnknown_08576877[gUnits[k].type] != 0)
            gUnknown_030045D0++;
    }

    gUnknown_030046D4 = CountUnitsWithTypeTag(1);
    AiConsiderBuildingFootUnit(a1);
    AiConsiderBuildingSupportUnits();
    AiConsiderBuildingCounterUnit();
    AiConsiderBuildingSubmarine();
    AiPickUnitToBuild();

    if (gUnknown_030046C0.unk06 != 0)
        AiCommitBuild();
}
asm(".global sub_08060718\n.thumb_set sub_08060718, AiBuildOneUnit\n");
