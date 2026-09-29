#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026D44.
 * sub_08026D44 @ 0x08026D44
 */

/* No narrowing on the index, so the parameter is a word and not the u8 its
 * siblings in this family take. The branched-to block is the FALSE arm, which
 * per the branch-polarity rule makes `return FALSE` the first return. */
bool8 DoesArmyHaveHq(int a)
{
    if ((gPlayers[a].hqX & 0x80) != 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_08026D44\n.thumb_set sub_08026D44, DoesArmyHaveHq\n");
