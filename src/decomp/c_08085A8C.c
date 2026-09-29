#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085A8C.
 * sub_08085A8C @ 0x08085A8C
 */

#include "hardware.h"
#include "proc.h"

/* One five-argument DrawOamObject call, then a key test that breaks the proc.
 * 0x103 is A | B | Start against gpKeySt->pressed (+0x04), and it is a pool word
 * because it does not fit a THUMB immediate.
 *
 * The parameter survives in r4 across the first call only to reach Proc_Break,
 * which is what makes it a ProcPtr rather than an unused argument. */
void CoInfoPopup_HoldLoop(ProcPtr proc)
{
    DrawOamObject(0x92, 0x38, 0x38, 0, 0);

    if (gpKeySt->pressed & (A_BUTTON | B_BUTTON | R_BUTTON))
        Proc_Break(proc);
}
asm(".global sub_08085A8C\n.thumb_set sub_08085A8C, CoInfoPopup_HoldLoop\n");
