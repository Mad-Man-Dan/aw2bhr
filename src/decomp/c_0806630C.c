#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806630C.
 * sub_0806630C @ 0x0806630C
 */

/* A two-case `switch` with an empty default, not an if/else chain: the case
 * bodies are laid out AFTER the literal pool with the dispatch falling through
 * to a shared `b`, which is gcc's switch layout. */
void MatchSetupRunStageLocal(void)
{
    switch (gUnknown_08580934->unk26)
    {
    case 0:
        MatchSetupHandleArmyStageInput();
        break;
    case 1:
        MatchSetupHandleTeamStageInput();
        break;
    }
}
asm(".global sub_0806630C\n.thumb_set sub_0806630C, MatchSetupRunStageLocal\n");
