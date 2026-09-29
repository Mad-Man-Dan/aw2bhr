#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026A48.
 * sub_08026A48 @ 0x08026A48, sub_08026A88 @ 0x08026A88, sub_08026AC0 @ 0x08026AC0, sub_08026B28 @ 0x08026B28
 */

/* Assigns every army slot 1..4 its team colour, but only in game modes 1 and 2.
 *
 * The guard is a SWITCH, not an `if`. `cmp #2; bgt` / `cmp #1; blt` is gcc's
 * switch range check, which uses the raw case values; every `if` spelling was
 * tried and none of them reproduces it:
 *   - `x >= 1 && x <= 2` on the u8 member folds to `(x - 1) <=u 1` (one
 *     subtract and one compare);
 *   - splitting it into nested ifs or two early returns keeps two compares but
 *     goes UNSIGNED and rewrites `>= 1` as `!= 0` (`cmp #0; beq`);
 *   - assigning to an `int` local first restores the signed `bgt` but folds
 *     `>= 1` to `> 0` (`cmp #0; ble`).
 * Only the switch emits the literal `cmp #1; blt`, because expand_case does not
 * go through fold.
 *
 * gUnknown_08090A80 in the asm is NOT a global: the ROM word at 0x08090A80
 * holds 0x08499598, agbcc's -fforce-addr address constant for
 * gPlayers. (Contrast gUnknown_08090A84 in BuildEnemyArmyMasks, which IS a
 * real table -- the prefix decides nothing.)
 *
 * The 0x3c running offset is strength reduction of the element stride. */
void AssignArmyTeamColors(void)
{
    int i;

    switch (gPlaySt.gameMode)
    {
    case 1:
    case 2:
        for (i = 1; i <= 4; i++)
            gPlayers[i].teamColor = PickArmyTeamColor(i, GetPlayerCoDefaultTeamColor(i));
        break;
    }
}
asm(".global sub_08026A48\n.thumb_set sub_08026A48, AssignArmyTeamColors\n");

/* Answers whether team colour `v` is still unused by army slots 1 .. n-1.
 *
 * The `n == 1` pre-test is a REAL early return, not a peeled iteration: the
 * `for` entry test (`movs r3,#1; cmp r3,r4; bge`) is still there right after
 * it, so the ROM tests the same thing twice and the source said it twice. */
bool8 IsTeamColorUnused(int n, int v)
{
    int i;

    if (n == 1)
        return 1;

    for (i = 1; i < n; i++)
    {
        if (gPlayers[i].teamColor == v)
            return 0;
    }

    return 1;
}
asm(".global sub_08026A88\n.thumb_set sub_08026A88, IsTeamColorUnused\n");

/* Picks the team colour for army slot `slot`. The chapter record supplies a
 * preset in unk3c[slot - 1] / unk40[slot - 1] -- 0xff in unk3c means "none",
 * in which case the caller's `fallback` is used. If that choice collides with
 * an already-assigned slot, it linearly searches colours 1..4 for a free one.
 *
 * The trailing `if (v > 5) v = 5;` is UNREACHABLE in practice -- the search
 * loop can only leave v at 5 -- but the ROM has the compare, so the source had
 * the clamp. It sits inside the collision branch: the non-colliding path
 * branches straight past it to the return. */
int PickArmyTeamColor(int slot, int fallback)
{
    int v;

    if (gUnknown_085C77A0[gPlaySt.mapID].unk3c[slot - 1] == 0xff)
        v = fallback;
    else
        v = gUnknown_085C77A0[gPlaySt.mapID].unk40[slot - 1];

    if (!IsTeamColorUnused(slot, v))
    {
        for (v = 1; v <= 4; v++)
        {
            if (IsTeamColorUnused(slot, v))
                break;
        }

        if (v > 5)
            v = 5;
    }

    return v;
}
asm(".global sub_08026AC0\n.thumb_set sub_08026AC0, PickArmyTeamColor\n");

/* Rebuilds each live army's "other armies I am at war with" mask: for army i,
 * OR in gUnknown_08090A84[j] for every j in 1..4 that is not i and is on a
 * different team (a different unk2a).
 *
 * gUnknown_08090A84 IS a real byte table here -- see include/unknown-globals.h.
 * Its neighbours 0x08090A70 / 0x08090A80 / 0x08090A8C are -fforce-addr address
 * words, so the shared prefix means nothing; the tell is that this one is
 * indexed straight off the symbol with no load through it.
 *
 * The two `ldr rN,=gPlayers` are one global read twice, not two
 * globals: the outer level parks the address in sb and the inner level needs
 * its own copy in r8.
 *
 * Both counters are u8 -- truncated `lsls/lsrs #0x18` each pass and compared
 * `bls`, unsigned -- and `i + 1` computed at the top of the outer body is
 * loop-optimiser output, not source. */
void BuildEnemyArmyMasks(void)
{
    u8 i;
    u8 j;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].aiControlled != 0)
        {
            for (j = 1; j <= 4; j++)
            {
                if (i != j && gPlayers[i].team != gPlayers[j].team)
                    gPlayers[i].unk2c |= gUnknown_08090A84[j];
            }
        }
    }
}
asm(".global sub_08026B28\n.thumb_set sub_08026B28, BuildEnemyArmyMasks\n");
