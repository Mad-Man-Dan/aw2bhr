#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080385A4.
 * sub_080385A4 @ 0x080385A4
 */

/* MATCHED byte-for-byte (wave 43, W43-E), first attempt.
 *
 * trymatch reports `relocs: name different symbols that resolve to the same
 * address -- original gUnknown_03003FF3, candidate gPlaySt`, which
 * is a match and needs nothing placed. gen_lds.py invents the name
 * gUnknown_03003FF3 for 0x03003FF3 because a pool word points there, but the
 * address is just &gPlaySt.armyColor[0] (0x03003FC0 + 0x33): agbcc folds
 * the member-array base into the pool as symbol+addend, so the honest
 * `gPlaySt.armyColor[i]` relocates to the same word. Do NOT declare a
 * global for it.
 *
 * The seeding loop's index is `s16`, not u8 and not int: each use is a
 * `lsls #0x10; asrs #0x10` sign-extension of a register agbcc keeps
 * zero-extended (PROMOTE_MODE), and the bound test is `ble`, signed. The
 * stored value is the index itself, so unk33[1..4] ends up holding 1,2,3,4 --
 * the identity seeding of the player-slot permutation the four parallel
 * 5-byte arrays at 0x33/0x38/0x3d/0x42 share.
 *
 * The three tails are plain statement sequences; the `adds r1,#1` chains
 * between the stores are agbcc CSEing the struct base forward, not evidence of
 * an array store.
 */

void SetDefaultArmySetup(void)
{
    s16 i;

    gPlayers[0].teamColor = 0;

    for (i = 1; i <= 4; i++)
        gPlaySt.armyColor[i] = i;

    gPlaySt.co[1] = 1;
    gPlaySt.co[2] = 3;
    gPlaySt.co[3] = 9;
    gPlaySt.co[4] = 6;

    if (gPlaySt.gameMode != 3)
    {
        gPlaySt.aiControlled[1] = 1;
        gPlaySt.aiControlled[2] = 2;
        gPlaySt.aiControlled[3] = 2;
        gPlaySt.aiControlled[4] = 2;
        SetFreeForAllTeams();
    }
}
asm(".global sub_080385A4\n.thumb_set sub_080385A4, SetDefaultArmySetup\n");
