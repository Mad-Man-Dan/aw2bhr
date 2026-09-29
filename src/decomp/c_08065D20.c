#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065D20.
 * sub_08065D20 @ 0x08065D20
 */

#include "hardware.h"

/* L or R (0xc0 of gpKeySt->repeated) flips the unk09[] mark of the slot unk32/2
 * selects between 1 and 2; if that leaves no slot marked 1, slot 1 -- or slot 0
 * when unk32/2 is not zero -- is forced back to 1. Then sound 0x64.
 *
 * gUnknown_0816E11C is agbcc's own -fforce-addr word holding &gUnknown_08580934
 * (the ROM word at 0x0816E11C is 0x08580934); the global is named honestly and
 * the pool word is left to the harness, as in MatchSetupUnpackRuleIndices / MatchSetupInitState /
 * MatchSetupDismissArmyColumns in this block.
 *
 * The unk09[k] flip is TWO ASSIGNMENT STATEMENTS, and nothing else reaches the
 * ROM's shape. Both arms are small constants, so `x == 1 ? 2 : 1` and
 * `if (x == 1) v = 2; else v = 1; unk09[k] = v;` BOTH collapse to
 * `movs rD,#1; cmp; bne; movs rD,#2` -- preset then conditionally overwrite,
 * no branch, 4 bytes short. Two separate stores keep the branchy form:
 * cross-jumping merges the identical `strb`s back into one, leaving the two
 * `movs` split across a `b`, which is the ROM. (c_08065818.c's `?:` keeps its
 * branch only because its true arm is not a constant.)
 *
 * Those four bytes also paid for the register allocation: with the collapsed
 * form CSE had a register spare and kept &gUnknown_08580934 live across the
 * search loop, costing a third callee-saved register and turning the loop's
 * two-level reload into one. Fixing the store shape fixed both -- they were
 * one fact, not two. */
void MatchSetupToggleController(void)
{
    int k;
    int i;

    k = gUnknown_08580934->unk32 / 2;

    if (gpKeySt->repeated & (DPAD_UP | DPAD_DOWN))
    {
        if (gUnknown_08580934->unk09[k] == 1)
            gUnknown_08580934->unk09[k] = 2;
        else
            gUnknown_08580934->unk09[k] = 1;

        for (i = 0; i < gUnknown_08580934->unk08; i++)
            if (gUnknown_08580934->unk09[i] == 1)
                break;

        if (i == gUnknown_08580934->unk08)
        {
            if (k == 0)
                gUnknown_08580934->unk09[1] = 1;
            else
                gUnknown_08580934->unk09[0] = 1;
        }

        PlayMusicOrSfx2(0x64);
    }
}
asm(".global sub_08065D20\n.thumb_set sub_08065D20, MatchSetupToggleController\n");
