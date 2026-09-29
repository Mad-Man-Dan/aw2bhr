#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080658AC.
 * sub_080658AC @ 0x080658AC
 */

/* The inverse of MatchSetupPackRuleIndices (src/decomp/c_08065818.c): that function packs the
 * header at +0x00 into the seven bytes at +0x84, this one unpacks them back.
 *
 * Two shapes here are NOT the `?:` the exemplar uses, and the ROM says so
 * directly. MatchSetupPackRuleIndices's `unk03 ? unk03 - 4 : 0` lays the NON-zero arm first
 * with the `b` to the tail; both stores here lay the ZERO arm first. `?:` folds
 * to the exemplar's order whichever way the condition is spelled (probed both),
 * so the zero-arm-first order is an if/else STATEMENT, and the +0x07 one keeps
 * two separate `strb` because the arms end up in different registers.
 *
 * `(u8)(unk88 - 1)` rather than `unk88 - 1`: the cast is what puts the constant
 * on the unk88 side as `adds #0xff` (shorten_binary_op narrows the add to
 * QImode). Written `unk88 - 1 + unk15` fold reassociates it to
 * `unk88 + (unk15 - 1)` and the `#0xff` lands on the freshly loaded unk15
 * instead -- same size, wrong two instructions. */
void MatchSetupUnpackRuleIndices(void)
{
    int i;

    gUnknown_08580934->unk00 = gUnknown_08580934->unk84 == 0;
    gUnknown_08580934->unk06 = gUnknown_08580934->unk85;
    gUnknown_08580934->unk04 = (gUnknown_08580934->unk86 + 2) * 500;
    gUnknown_08580934->unk01 = gUnknown_08580934->unk89 == 0;
    gUnknown_08580934->unk02 = gUnknown_08580934->unk8a;
    if (gUnknown_08580934->unk87 == 0)
        gUnknown_08580934->unk03 = 0;
    else
        gUnknown_08580934->unk03 = gUnknown_08580934->unk87 + 4;
    if (gUnknown_08580934->unk88 == 0)
        gUnknown_08580934->unk07 = 0;
    else
        gUnknown_08580934->unk07 = (u8)(gUnknown_08580934->unk88 - 1) + gUnknown_08580934->unk15;

    for (i = 0; i < 4; i++)
        gUnknown_08580934->unk20[i] = gUnknown_08580934->unk18[gUnknown_08580934->unk1c[i]];
}
asm(".global sub_080658AC\n.thumb_set sub_080658AC, MatchSetupUnpackRuleIndices\n");
