#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026924.
 * sub_08026924 @ 0x08026924
 */

/* Chapter setup: clears the unused army slots' gUnknown_03003FF8 entries, then
 * copies the four per-slot arrays out of gPlaySt into the army
 * records, and finally walks the unit table handing every live unit to
 * IncrementPlayerUnitsCreated.
 *
 * TWO READ-OUTS THAT ARE NOT FREE:
 *
 * 1. The first loop really is DESCENDING in the source. Its counter runs down
 *    from `4 - GetLoadedMapArmyCount()` to 1 and the index is written `5 - k`, which is
 *    why the constant 5 is a live register across it. Rewriting it as the
 *    equivalent ascending `for (i = n + 1; i <= 4; i++)` does NOT get rewritten
 *    back into this by check_dbra_loop -- it produces an ascending loop with a
 *    direct index and a `cmp #4` bottom test, 30 bytes different. So this is a
 *    case where the down-counter is the source and not the optimiser, which is
 *    the opposite of the usual warning.
 *
 * 2. `j = 5 - k;` MUST be its own statement. Folded into the subscript as
 *    `gUnknown_03003FF8[5 - k]` everything matches except that agbcc hoists the
 *    array's address into the preheader BEFORE the constant 5, swapping the two
 *    registers (`ldr r4, =base; movs r3, #5` against the ROM's `movs r4, #5;
 *    ldr r3, =base`) -- 7 bytes on an otherwise size-exact 97.6% candidate.
 *    Splitting the index out creates the constant's pseudo first and the
 *    allocation order follows. Same lever as the wave-17 allocno rule, reached
 *    from the statement side.
 *
 * `i` is s16 throughout: every loop increment is `lsls #0x10 / lsrs #0x10 /
 * asrs #0x10` with the SIGN-extended copy feeding the compare. The last loop's
 * argument `(i >> 6) + 1` is a real arithmetic shift (`asrs r0, r2, #0x16` on
 * the already-shifted value), not a division -- a signed /64 would carry a
 * rounding correction. */
void InitPlayersFromSettings(void)
{
    s16 i;
    s16 k;
    int j;

    for (k = 4 - GetLoadedMapArmyCount(); k > 0; k--)
    {
        j = 5 - k;
        gUnknown_03003FF8[j] = 0;
    }

    gPlayers[1].aiControlled = gPlaySt.aiControlled[1];
    gPlayers[2].aiControlled = gPlaySt.aiControlled[2];
    gPlayers[3].aiControlled = gPlaySt.aiControlled[3];
    gPlayers[4].aiControlled = gPlaySt.aiControlled[4];

    for (i = 1; i <= 4; i++)
    {
        gPlayers[i].teamColor = gPlaySt.armyColor[i];
        gPlayers[i].co = gPlaySt.co[i];
        gPlayers[i].team = gPlaySt.unk42[i];
        gPlayers[i].unk2c = 0;

        if (gPlayers[i].aiControlled == 0)
            gPlayers[i].co = 1;
    }

    AssignArmyTeamColors();

    if (gPlaySt.coAbilities == 0)
        gPlaySt.coPowersEnabled = 0;

    BuildEnemyArmyMasks();

    for (i = 1; i <= 0xff; i++)
    {
        if (gUnits[i].type != 0)
            IncrementPlayerUnitsCreated((i >> 6) + 1);
    }
}
asm(".global sub_08026924\n.thumb_set sub_08026924, InitPlayersFromSettings\n");
