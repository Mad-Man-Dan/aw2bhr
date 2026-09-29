#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08058254.
 * AiCountFriendlyArmedVehiclesInReach @ 0x08058254, AiCountFriendlyArmedVehiclesInReach2 @ 0x08058318
 */

/* MATCHED, first draft, one attempt, and the first of a byte-identical pair
 * with AiCountFriendlyArmedVehiclesInReach2. Counts the deployed units of every army that is not masked
 * out: for each of the four armies, skip it if bit i of
 * gPlayers[gUnknown_030033EC].unk2c is set, then scan that army's 64
 * slots and count the ones that are a real unit of class 2, whose type has a
 * non-zero gUnknown_085D5ABC cap, and that are actually standing on the map.
 *
 * Three readouts worth keeping:
 *
 *  - The inner loop is `for (j = i * 64; j < i * 64 + 64; j++)`, not
 *    `for (j = 0; j < 64; j++)` over `[i * 64 + j]`. The ROM has the loop's
 *    ENTRY GUARD (`cmp r3, r4; bge`) comparing `i*64` against `i*64+64` -- a
 *    test that is always false and that gcc only emits when it cannot fold the
 *    bounds. A constant-bounded inner loop has no guard at all.
 *  - `unk2c` is a plain byte shifted by a VARIABLE (`asrs r1, r2`), so it is a
 *    u8 field and not a bitfield; a bitfield would have been a constant shift
 *    pair.
 *  - The map read is `(s8)gUnknown_03003340[row][col] == -1`. The `lsls #0x18;
 *    asrs #0x18` is the cast, and -1 lives in sb for the whole function because
 *    it is a loop-invariant constant, not because anything in the source binds
 *    it.
 *
 * gUnknown_030033EC's load is hoisted out of the outer loop and
 * gPlayers's deref is NOT, although both are invariant. That falls out
 * of gcc's own invariant motion and needs nothing in the source. */
int AiCountFriendlyArmedVehiclesInReach(void)
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

            if (u->type <= 2)
                continue;
            if (gUnknown_0857680F[u->type] != 2)
                continue;
            if (gUnknown_085D5ABC[u->type].maxAmmo == 0)
                continue;
            if ((s8)gUnknown_03003340[u->y][u->x] == -1)
                continue;

            count++;
        }
    }

    return count;
}
asm(".global sub_08058254\n.thumb_set sub_08058254, AiCountFriendlyArmedVehiclesInReach\n");

/* MATCHED. Byte-for-byte the same function as AiCountFriendlyArmedVehiclesInReach -- same loops, same
 * predicates, same pool. Read that one for the loop-bound readout. */
int AiCountFriendlyArmedVehiclesInReach2(void)
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

            if (u->type <= 2)
                continue;
            if (gUnknown_0857680F[u->type] != 2)
                continue;
            if (gUnknown_085D5ABC[u->type].maxAmmo == 0)
                continue;
            if ((s8)gUnknown_03003340[u->y][u->x] == -1)
                continue;

            count++;
        }
    }

    return count;
}
asm(".global sub_08058318\n.thumb_set sub_08058318, AiCountFriendlyArmedVehiclesInReach2\n");
