#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D8B8.
 * sub_0806D8B8 @ 0x0806D8B8
 */

/* The +0x84 packed header, the descending twin of MatchSetupPackRuleIndices's. The `(u8)`
 * on `unk07 + 1` is load-bearing and is the whole difference between 92% and a
 * match: without it agbcc reassociates `a + 1 - b` into `a - (b - 1)` and
 * loads unk15 FIRST (probed side by side -- `(a + 1) - b` and `a - b + 1` both
 * give the reassociated form). The cast pins the `+ 1` to its own instruction
 * ahead of the second load, which is what the ROM has. */
void RulesScreenPackRuleIndices(void)
{
    gUnknown_08580934->unk84 = (gUnknown_08580934->unk00 == 0);
    gUnknown_08580934->unk85 = gUnknown_08580934->unk06;
    gUnknown_08580934->unk86 = gUnknown_08580934->unk04 / 500 - 2;
    gUnknown_08580934->unk87 =
        (gUnknown_08580934->unk03 != 0) ? gUnknown_08580934->unk03 - 4 : 0;
    gUnknown_08580934->unk88 = (gUnknown_08580934->unk07 != 0)
        ? (u8)(gUnknown_08580934->unk07 + 1) - gUnknown_08580934->unk15 : 0;
    gUnknown_08580934->unk89 = (gUnknown_08580934->unk01 == 0);
    gUnknown_08580934->unk8a = gUnknown_08580934->unk02;
}
asm(".global sub_0806D8B8\n.thumb_set sub_0806D8B8, RulesScreenPackRuleIndices\n");
