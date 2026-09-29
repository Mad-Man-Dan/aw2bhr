#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D92C.
 * sub_0803D92C @ 0x0803D92C
 */

/* The local is `int`, NOT `s8`. GetSuspendIdForGameMode returns s8, so agbcc re-narrows
 * its result at the call site -- that single `lsls #0x18; asrs #0x18` into r4
 * IS the narrowing, and r4 is then used sign-extended everywhere. Declaring the
 * local `s8` makes agbcc truncate with `lsls; lsrs` into the local and
 * sign-extend again at each read, four bytes longer. */
void SaveScreenCampaign_StartMessage(void)
{
    int v;

    v = GetSuspendIdForGameMode(1);
    if (v != 0)
        SetSuspendFlag(v, 0);
    if (IsPlayer1TeamAlive())
        StartEventScript(gUnknown_0849F3A8)->unk10 = v;
}
asm(".global sub_0803D92C\n.thumb_set sub_0803D92C, SaveScreenCampaign_StartMessage\n");
