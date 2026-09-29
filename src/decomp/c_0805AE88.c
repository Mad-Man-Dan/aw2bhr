#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805AE88.
 * sub_0805AE88 @ 0x0805AE88, sub_0805AF90 @ 0x0805AF90, sub_0805B0AC @ 0x0805B0AC, sub_0805B1CC @ 0x0805B1CC, sub_0805B2EC @ 0x0805B2EC
 */

/* Map-redraw family, 0x0805A/0x0805B. Same shape as AiMarkLandingCellsNearEnemyProperties (see
 * work/AiMarkLandingCellsNearEnemyProperties) with exactly four measured differences, and this file is
 * the one that never returns: it has NO epilogue at all, its bottom being
 * `bl AiFinishLandingPlan; b <top>`.
 *
 * The four, against AiMarkLandingCellsNearEnemyProperties:
 *   1. AiNextEnemyPropertyInInterestList returning false calls AiMarkLandingCellsNearEnemyProperties() and falls through,
 *      where AiMarkLandingCellsNearEnemyProperties itself returns.
 *   2. no epilogue -- consequence of (1) leaving the loop no exit.
 *   3. the indirect hook's third argument is the literal 1, not
 *      gUnknown_030046D4, which is why this function's data_refs lack it. The
 *      `movs r2,#1` lands AFTER the two pool loads because argument setup is
 *      grouped by operand class (copies, pool ldrs, then mov #imm8), not by
 *      argument order.
 *   4. the loop-continue test is AiCountFriendlyArmedVehiclesInReach2() < AiCountFriendlyFootUnitsInReach(), with no `+ 5`.
 * Everything else that differs in the listings is register choice and is not
 * reachable from the source.
 *
 * The double loop is c_0805AD90.c's AiMarkLandingCellsNearEnemyHq verbatim -- see that file's
 * header for why `rows` must be its own local and why the `do { } while (0)`
 * around the two pointer bumps is a register-allocation lever, not control
 * flow. MATCHED first attempt. */
void AiMarkLandingCellsNeedingEscort(void)
{
    int it;
    int i;
    int a;
    int b;
    int x;
    int y;
    int off;
    int terrain;

    i = -1;
    it = sub_0805B4A8();

    for (;;)
    {
        i++;
        if ((u8)AiNextEnemyPropertyInInterestList(&it, &i, &a, &b) == 0)
            AiMarkLandingCellsNearEnemyProperties();

        gUnknown_030013EC(a, b, 1, gUnknown_085766E0->unk0f, 0);

        if (AiCountFriendlyArmedVehiclesInReach2() < AiCountFriendlyFootUnitsInReach())
        {
            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] >= 0)
                    {
                        off = gMap->rowOffset[y] + x;
                        terrain = gMap->terrain[off] & 0x1f;
                        if (terrain == 0xd || terrain == 0xb)
                        {
                            gMap->unk3C72[off]++;
                        }
                    }
                }
            }

            AiFinishLandingPlan();
        }
    }
}
asm(".global sub_0805AE88\n.thumb_set sub_0805AE88, AiMarkLandingCellsNeedingEscort\n");

/* Map-redraw family, 0x0805A/0x0805B, and the representative the other four
 * were derived from. Loops on AiNextEnemyPropertyInInterestList (the AiFindEnemyHqInInterestList twin that walks a
 * list, seeded from sub_0805B4A8()), hands its two out-params to the
 * gUnknown_030013EC indirect hook, and -- while the frame budget allows --
 * sweeps every passable cell and bumps the per-cell counter in the map's 0x3C72
 * plane wherever the terrain code is 0xd or 0xb.
 *
 * `i` is preset to -1 and bumped at the top of the loop, so the first
 * AiNextEnemyPropertyInInterestList call sees 0. The three later argument addresses (&i, &a, &b) are
 * LICM-hoisted into sp+0x14, sp+0x18 and sl -- compiler output, not source.
 * AiNextEnemyPropertyInInterestList's result is truth-tested one byte wide; see the note added at
 * its declaration in unknown-functions.h for why the `(u8)` is written as a
 * cast at the call rather than by retyping the shared prototype.
 *
 * The double loop is c_0805AD90.c's AiMarkLandingCellsNearEnemyHq verbatim -- see that file's
 * header for why `rows` must be its own local (else agbcc reassociates the
 * 0x417A pool constant to last) and why the `do { } while (0)` around the two
 * pointer bumps is a register-allocation lever, not control flow.
 *
 * MATCHED first attempt, straight off the exemplar with no probing. */
void AiMarkLandingCellsNearEnemyProperties(void)
{
    int it;
    int i;
    int a;
    int b;
    int x;
    int y;
    int off;
    int terrain;

    i = -1;
    it = sub_0805B4A8();

    for (;;)
    {
        i++;
        if ((u8)AiNextEnemyPropertyInInterestList(&it, &i, &a, &b) == 0)
            return;

        gUnknown_030013EC(a, b, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

        if (AiCountFriendlyArmedVehiclesInReach2() < AiScoreEnemyPropertiesInReach() + 5)
        {
            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] >= 0)
                    {
                        off = gMap->rowOffset[y] + x;
                        terrain = gMap->terrain[off] & 0x1f;
                        if (terrain == 0xd || terrain == 0xb)
                        {
                            gMap->unk3C72[off]++;
                        }
                    }
                }
            }

            AiFinishLandingPlan();
        }
    }
}
asm(".global sub_0805AF90\n.thumb_set sub_0805AF90, AiMarkLandingCellsNearEnemyProperties\n");

/* Map-redraw family, 0x0805A/0x0805B. Returning variant, like AiMarkLandingCellsNearEnemyProperties:
 * the driver returning false is an early `return` and the function has a real
 * epilogue. It drives on AiNextInterestListEntry and its loop-continue test is the
 * TWO-PART form that distinguishes this pair from AiMarkLandingCellsNearEnemyProperties's single
 * comparison:
 *
 *     n = AiCountEnemyLandUnitsInReach();
 *     if (n != 0 && AiCountFriendlyArmedVehiclesInReach() < n + 5)
 *
 * -- the `+ K` operand is evaluated FIRST, held in r4, zero-tested, and only
 * then compared against the second query. That forces `n` to be a real local;
 * AiMarkLandingCellsNearEnemyProperties's one-comparison form needs none. Note this is the same pair of
 * counters and the same `+ 5` that unknown-functions.h records at sub_0805BEA0,
 * with the operands the other way round.
 *
 * The two LICM-hoisted argument addresses land in sp+0x18 and sp+0x14 here,
 * swapped against AiMarkLandingCellsNearEnemyProperties's sp+0x14 and sp+0x18. That is the loop
 * optimiser reacting to `n` being live across the preheader -- compiler output,
 * not source, and it falls out on its own.
 *
 * The double loop is c_0805AD90.c's AiMarkLandingCellsNearEnemyHq verbatim -- see that file's
 * header for why `rows` must be its own local and why the `do { } while (0)`
 * around the two pointer bumps is a register-allocation lever, not control
 * flow. MATCHED first attempt. */
void AiMarkLandingCellsNearEnemyUnits(void)
{
    int it;
    int i;
    int a;
    int b;
    int n;
    int x;
    int y;
    int off;
    int terrain;

    i = -1;
    it = sub_0805B4A8();

    for (;;)
    {
        i++;
        if ((u8)AiNextInterestListEntry(&it, &i, &a, &b) == 0)
            return;

        gUnknown_030013EC(a, b, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

        n = AiCountEnemyLandUnitsInReach();
        if (n != 0 && AiCountFriendlyArmedVehiclesInReach() < n + 5)
        {
            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] >= 0)
                    {
                        off = gMap->rowOffset[y] + x;
                        terrain = gMap->terrain[off] & 0x1f;
                        if (terrain == 0xd || terrain == 0xb)
                        {
                            gMap->unk3C72[off]++;
                        }
                    }
                }
            }

            AiFinishLandingPlan();
        }
    }
}
asm(".global sub_0805B0AC\n.thumb_set sub_0805B0AC, AiMarkLandingCellsNearEnemyUnits\n");

/* Map-redraw family, 0x0805A/0x0805B. AiMarkLandingCellsNearEnemyUnits's twin, instruction for
 * instruction: same returning shape with a real epilogue, same two-part
 * loop-continue test with the `+ K` operand evaluated first into r4 and
 * zero-tested before the second query. It differs from AiMarkLandingCellsNearEnemyUnits in exactly
 * three tokens -- it drives on AiNextEnemyPropertyInInterestList rather than AiNextInterestListEntry, its two
 * counters are AiScoreEnemyPropertiesInReach and AiCountFriendlyFootUnitsInReach rather than AiCountEnemyLandUnitsInReach and
 * AiCountFriendlyArmedVehiclesInReach, and the margin is `+ 2` rather than `+ 5`.
 *
 * The double loop is c_0805AD90.c's AiMarkLandingCellsNearEnemyHq verbatim -- see that file's
 * header for why `rows` must be its own local and why the `do { } while (0)`
 * around the two pointer bumps is a register-allocation lever, not control
 * flow. MATCHED first attempt. */
void AiMarkLandingCellsNeedingCapturers(void)
{
    int it;
    int i;
    int a;
    int b;
    int n;
    int x;
    int y;
    int off;
    int terrain;

    i = -1;
    it = sub_0805B4A8();

    for (;;)
    {
        i++;
        if ((u8)AiNextEnemyPropertyInInterestList(&it, &i, &a, &b) == 0)
            return;

        gUnknown_030013EC(a, b, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

        n = AiScoreEnemyPropertiesInReach();
        if (n != 0 && AiCountFriendlyFootUnitsInReach() < n + 2)
        {
            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] >= 0)
                    {
                        off = gMap->rowOffset[y] + x;
                        terrain = gMap->terrain[off] & 0x1f;
                        if (terrain == 0xd || terrain == 0xb)
                        {
                            gMap->unk3C72[off]++;
                        }
                    }
                }
            }

            AiFinishLandingPlan();
        }
    }
}
asm(".global sub_0805B1CC\n.thumb_set sub_0805B1CC, AiMarkLandingCellsNeedingCapturers\n");

/* Map-redraw family, 0x0805A/0x0805B. AiMarkLandingCellsNeedingEscort's twin: same no-epilogue
 * shape, same `if (!driver(...)) AiMarkLandingCellsNearEnemyProperties();` fall-through. It differs from
 * AiMarkLandingCellsNeedingEscort in only two things -- it drives on AiNextInterestListEntry rather than
 * AiNextEnemyPropertyInInterestList (the same signature, minus the terrain predicate), and its
 * loop-continue test is a bare truth test, `sub_080586CC() != 0`, with no
 * second query and no `+ K`. Its indirect hook takes gUnknown_030046D4 as the
 * third argument the way AiMarkLandingCellsNearEnemyProperties does, not AiMarkLandingCellsNeedingEscort's literal 1.
 *
 * The double loop is c_0805AD90.c's AiMarkLandingCellsNearEnemyHq verbatim -- see that file's
 * header for why `rows` must be its own local and why the `do { } while (0)`
 * around the two pointer bumps is a register-allocation lever, not control
 * flow. MATCHED first attempt (the one failed run was a missing prototype for
 * sub_080586CC, now published in unknown-functions.h from its promoted
 * definition in src/decomp/c_080586CC.c). */
void sub_0805B2EC(void)
{
    int it;
    int i;
    int a;
    int b;
    int x;
    int y;
    int off;
    int terrain;

    i = -1;
    it = sub_0805B4A8();

    for (;;)
    {
        i++;
        if ((u8)AiNextInterestListEntry(&it, &i, &a, &b) == 0)
            AiMarkLandingCellsNearEnemyProperties();

        gUnknown_030013EC(a, b, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

        if (sub_080586CC() != 0)
        {
            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] >= 0)
                    {
                        off = gMap->rowOffset[y] + x;
                        terrain = gMap->terrain[off] & 0x1f;
                        if (terrain == 0xd || terrain == 0xb)
                        {
                            gMap->unk3C72[off]++;
                        }
                    }
                }
            }

            AiFinishLandingPlan();
        }
    }
}
