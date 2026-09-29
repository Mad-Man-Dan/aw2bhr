#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078ED4.
 * sub_08078ED4 @ 0x08078ED4
 */

#include "proc.h"

/* Picks one of two scripts on gPlaySt.gameMode == 3 -- the mode
 * selector documented on that struct, whose value 3 is written by
 * sub_0803B8C4. Both arms forward the incoming parent unchanged, which is why
 * `adds r1, r0, #0` is the only thing in the prologue. */

void StartResultsSubScreen(ProcPtr parent)
{
    if (gPlaySt.gameMode == 3)
        Proc_Start(gUnknown_08615DD8, parent);
    else
        Proc_Start(gUnknown_08615D88, parent);
}
asm(".global sub_08078ED4\n.thumb_set sub_08078ED4, StartResultsSubScreen\n");
