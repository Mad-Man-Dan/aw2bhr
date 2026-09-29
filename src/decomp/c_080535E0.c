#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080535E0.
 * sub_080535E0 @ 0x080535E0
 */

/* StepBattleHud is called from BOTH arms, and that is the source and not a
 * codegen artefact: the ROM contains two separate `bl StepBattleHud`. Written as
 * one test (`if (!bit0 || bit1)`) there would be a single call site.
 *
 * The flag byte is loaded ONCE and both masks are applied to that register --
 * ordinary CSE, since nothing intervenes. */
void StepBattleHudAndPanelScroll(void)
{
    if (gUnknown_03004504.bit0)
    {
        if (gUnknown_03004504.bit1)
            StepBattleHud();
    }
    else
    {
        StepBattleHud();
    }

    StepPanelScrollDriver(0);
    StepPanelScrollDriver(1);
}
asm(".global sub_080535E0\n.thumb_set sub_080535E0, StepBattleHudAndPanelScroll\n");
