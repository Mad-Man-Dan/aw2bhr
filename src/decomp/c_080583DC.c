#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080583DC.
 * sub_080583DC @ 0x080583DC, sub_0805848C @ 0x0805848C, sub_08058530 @ 0x08058530
 */

/* Counts the deployed units of every army that IS masked out -- the inverse of
 * AiCountFriendlyArmedVehiclesInReach's army selection. Three differences from that exemplar, and
 * they are the whole function:
 *
 *  - The army-mask test is INVERTED (`cmp r1,#0; beq` where the exemplar has
 *    `bne`), so this counts the armies whose bit in
 *    gPlayers[gUnknown_030033EC].unk2c is SET.
 *  - The unit predicate is `unk00 == 0` (`ldrb; cmp #0; beq`), not `<= 2`.
 *  - The gUnknown_085D5ABC[unk00].unk0b cap test is absent, which is why this
 *    function carries five pool words to the exemplar's six.
 *
 * Everything else is transcribed from c_08058254.c: the inner loop is
 * `for (j = i * 64; j < i * 64 + 64; j++)` (the ROM's always-false entry guard
 * `cmp r3, r4; bge` is the proof -- a constant-bounded loop has no guard),
 * unk2c is a plain u8 shifted by a variable rather than a bitfield, and the
 * `lsls #0x18; asrs #0x18` pair on the map read IS the `(s8)` cast.
 *
 * MATCHED first draft. */
int AiCountEnemyLandUnitsInReach(void)
{
    int count;
    int i;
    int j;
    struct Unit *u;

    count = 0;

    for (i = 0; i < 4; i++)
    {
        if (((gPlayers[gUnknown_030033EC].unk2c >> i) & 1) == 0)
            continue;

        for (j = i * 64; j < i * 64 + 64; j++)
        {
            u = &gUnits[j];

            if (u->type == 0)
                continue;
            if (gUnknown_0857680F[u->type] != 2)
                continue;
            if ((s8)gUnknown_03003340[u->y][u->x] == -1)
                continue;

            count++;
        }
    }

    return count;
}
asm(".global sub_080583DC\n.thumb_set sub_080583DC, AiCountEnemyLandUnitsInReach\n");

/* Counts the units of the NOT-masked-out armies whose unk00 is 1 or 2. Same
 * skeleton as c_08058254.c; two differences, both in the predicate block:
 *
 *  - The unit test is a RANGE, `(u8)(unk00 - 1) > 1` -- the ROM's
 *    `subs #1; lsls #0x18; lsrs #0x18; cmp #1; bhi`. The u8 truncation of the
 *    DIFFERENCE plus an unsigned compare is how a two-value id range is
 *    spelled with one branch (same idiom as c_0805CC88.c's
 *    `(u8)(unk00 - 0x10) <= 1`); a plain int `unk00 - 1 > 1` would drop the
 *    shift pair.
 *  - The gUnknown_0857680F class test and the gUnknown_085D5ABC cap test are
 *    both absent -- four pool words to the exemplar's six.
 *
 * The army mask stays INLINE in the loop condition even though the ROM hoists
 * its whole address computation into the preheader: that hoist is gcc's own
 * LICM, not a source-level local. Binding it to a local before the loop is
 * observably different code -- it sinks the `movs r5, #0` loop init BELOW the
 * hoisted block and moves gUnits's address into a callee-saved
 * register. The ROM's order (`movs r7,#0; movs r5,#0` and only then the mask
 * expression) is the tell that the for-init precedes the preheader.
 *
 * MATCHED. */
int AiCountFriendlyFootUnitsInReach(void)
{
    int count;
    int i;
    int j;
    struct Unit *u;

    count = 0;

    for (i = 0; i < 4; i++)
    {
        if ((gPlayers[gUnknown_030033EC].unk2c >> i) & 1)
            continue;

        for (j = i * 64; j < i * 64 + 64; j++)
        {
            u = &gUnits[j];

            if ((u8)(u->type - 1) > 1)
                continue;
            if ((s8)gUnknown_03003340[u->y][u->x] == -1)
                continue;

            count++;
        }
    }

    return count;
}
asm(".global sub_0805848C\n.thumb_set sub_0805848C, AiCountFriendlyFootUnitsInReach\n");

/* The inverted-mask twin of AiCountFriendlyFootUnitsInReach: identical in every instruction
 * except the army-mask branch, which is `beq` here and `bne` there, so this
 * one counts the units of the armies that ARE masked out. Read c_0805848C's
 * header for the `(u8)(unk00 - 1) > 1` range idiom and for why the mask
 * expression must stay inline rather than being bound to a local.
 *
 * MATCHED. */
int sub_08058530(void)
{
    int count;
    int i;
    int j;
    struct Unit *u;

    count = 0;

    for (i = 0; i < 4; i++)
    {
        if (((gPlayers[gUnknown_030033EC].unk2c >> i) & 1) == 0)
            continue;

        for (j = i * 64; j < i * 64 + 64; j++)
        {
            u = &gUnits[j];

            if ((u8)(u->type - 1) > 1)
                continue;
            if ((s8)gUnknown_03003340[u->y][u->x] == -1)
                continue;

            count++;
        }
    }

    return count;
}
