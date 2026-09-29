#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08081334.
 * sub_08081334 @ 0x08081334
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Start one script unless a different one is already running. The two blobs are
 * NOT the same object -- Proc_Find looks for gUnknown_08616A40 and Proc_Start
 * launches ProcScr_MainMenuC2 -- so this is a mutual exclusion between two
 * trees, not the usual "start it once" idiom. */
void MainMenuCarousel_StartWheelProc(ProcPtr parent)
{
    if (Proc_Find(gUnknown_08616A40) == 0)
        Proc_Start(ProcScr_MainMenuC2, parent);
}

asm(".global sub_08081334\n.thumb_set sub_08081334, MainMenuCarousel_StartWheelProc\n");
