#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A454.
 * sub_0806A454 @ 0x0806A454
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Resets the sub_0806A054 ticket counter and starts the screen's proc on
 * tree 3. The Proc_Start result is discarded (`pop {r0}; bx r0`). */
void StartIntroSequence(void)
{
    StopAllMusic();
    gUnknown_0202F204 = 0;
    Proc_Start(ProcScr_IntroT3, PROC_TREE_3);
}

asm(".global sub_0806A454\n.thumb_set sub_0806A454, StartIntroSequence\n");
