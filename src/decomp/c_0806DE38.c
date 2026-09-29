#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806DE38.
 * sub_0806DE38 @ 0x0806DE38
 */

#include "hardware.h"

/* The `(s16)` on unk2a is load-bearing and is the whole 41.8%-vs-match diff:
 * the member is declared u16, and a bare read gives `ldrh r0,[r0,#0x2a]` where
 * the ROM has `movs r1,#0x2a; ldrsh r0,[r0,r1]`. Probed: an (s16) cast on a
 * u16 STRUCT MEMBER folds into a sign-extending load rather than expanding to
 * lsls/asrs, so it costs nothing and does not need the shared member retyped.
 */
void RulesScreenHandleRulesPageInput(void)
{
    int scale;

    gUnknown_08580934->unk2a++;

    RulesScreenHandleMenuInput();

    if (gUnknown_0202F2C8 == 0)
    {
        RulesScreenRuleOption_ChangeValue((struct Unk0806DD34 *)
            gUnknown_08580934->unk54[gUnknown_08580934->unk33]);
        RulesScreenRuleOption_DrawArrows(gUnknown_08580934->unk33);
    }

    RulesScreenHighlightSelectedRuleOption();

    scale = (SinDegrees(((s16)gUnknown_08580934->unk2a * 16) % 360) >> 9) + 0x100;

    SetObjAffine(0,
        Div(gSinLut[0x40] * 16, scale != 0 ? scale : 2),
        Div(-gSinLut[0] * 16, scale != 0 ? scale : 2),
        Div(gSinLut[0] * 16, scale != 0 ? scale : 2),
        Div(gSinLut[0x40] * 16, scale != 0 ? scale : 2));
}
asm(".global sub_0806DE38\n.thumb_set sub_0806DE38, RulesScreenHandleRulesPageInput\n");
