#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066FE0.
 * sub_08066FE0 @ 0x08066FE0
 */

/* The two guards are ONE `&&`: the pointer is loaded once into r1 and survives
 * the gUnknown_0202F200 test to serve the unk30 read, which is what a single
 * short-circuit expression looks like -- nested `if`s would reload it. */
void MatchSetupUpdate(void)
{
    MatchSetupCyclePaletteColors();

    if (gUnknown_08580934->unk2d == 0 && gUnknown_0202F200 != 1)
    {
        if (gUnknown_08580934->unk30 == 1)
            MatchSetupRunStage();
        else
            MatchSetupHandleRulesStageInput();

        MatchSetupShowHelpText();
    }
}
asm(".global sub_08066FE0\n.thumb_set sub_08066FE0, MatchSetupUpdate\n");
