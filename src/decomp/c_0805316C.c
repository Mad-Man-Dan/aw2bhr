#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805316C.
 * sub_0805316C @ 0x0805316C
 */

#include "proc.h"

/* One frame of the cutscene player, ending the proc after 0x12C of them.
 *
 * `++gUnknown_03004508 == 0x12C` and not a re-read: the ROM compares the
 * INCREMENTED value still in its register, truncating it with a lone
 * `lsls #0x10` and comparing against `0x96 << 0x11` -- i.e. 0x12C already
 * shifted left 16. That paired shift is agbcc's u16 equality test done in the
 * shifted domain, which needs no `lsrs`; a reload would have emitted one. */
void sub_0805316C(ProcPtr proc)
{
    RunBattleAnimStepHandler();
    StepPanelBounceBothSides();

    if (++gUnknown_03004508 == 0x12C)
        Proc_Break(proc);
}
