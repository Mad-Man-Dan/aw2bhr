#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020824.
 * sub_08020824 @ 0x08020824
 */

/* Three-valued: 0 if the second record is empty, 2 if the two records agree on
 * unk2a, 1 otherwise. The `lsls #4; subs; lsls #2` chain is x * 15 * 4, agbcc's
 * shape for gPlayers's 0x3c stride.
 *
 * `adds rN, #0x2a` on both bases rather than a displacement: 0x2a is past
 * ldrb's imm5 limit of 31.
 *
 * The `return 1` comes FIRST in the source and LAST in the layout -- that
 * inversion is what a single condition does, and writing the equality test
 * first puts the constants the other way round. */
int GetArmyTeamRelation(u16 a, u16 b)
{
    if (gPlayers[b].aiControlled == 0)
        return 0;

    if (gPlayers[a].team != gPlayers[b].team)
        return 1;

    return 2;
}
asm(".global sub_08020824\n.thumb_set sub_08020824, GetArmyTeamRelation\n");
