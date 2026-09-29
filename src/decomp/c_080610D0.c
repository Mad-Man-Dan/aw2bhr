#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080610D0.
 * sub_080610D0 @ 0x080610D0
 */

/* AiCommitBuild @ 0x080610D0, 168 bytes.
 *
 * The ONLY difference between this and the obvious spelling is where the
 * constant 0 shared by the two `strb`s is materialised, and it is worth one
 * callee-saved register: the ROM sets it with `movs r1, #0` AFTER the
 * BuyUnit call and pushes {r4, r5, lr}, while the plain version creates the
 * pseudo before the two intervening calls, so it has to live in r6 and the
 * function pushes {r4, r5, r6, lr}. Instruction count and size are identical
 * either way -- the 31 differing bytes are register numbers.
 *
 * Probed directly: with only ONE `= 0` store the constant is materialised after
 * the call and the push list is right, so it is the SECOND use that creates the
 * long-lived pseudo. Neither `unit->unk0a = unit->unk09;`, nor
 * `unit->unk0a = unit->unk09 = 0;`, nor a `u8 z = 0;` local, nor swapping the
 * two stores, nor making `pos` a union moves it back.
 *
 * The `do { } while (0)` around the PickWeightedAiUnit store is what does move it
 * (found by decomp-permuter): it ends the basic block before the
 * AiPickBuildCell call, so the constant cannot be hoisted above it. It is
 * standing in for whatever the original had there -- a macro, most likely --
 * and it is byte-neutral apart from that one effect.
 */
void AiCommitBuild(void)
{
    struct Unk802C57C pos;
    struct Unit *unit;

    if (GetUnitCostWithCoBonus(gUnknown_030033EC, gUnknown_030046C0.unk06) * 10
            <= gPlayers[gUnknown_030033EC].funds
        && gUnknown_03004674 <= 0x3f)
    {
        do
        {
            gUnknown_030046C0.unk07 = PickWeightedAiUnit(gUnknown_030046C0.unk06 - 1);
        } while (0);

        if (AiPickBuildCell(&pos))
        {
            unit = BuyUnit(pos.unk00, pos.unk02, gUnknown_030046C0.unk06);
            unit->unk09 = 0;
            unit->unk0a = 0;
            unit->unk0b = gUnknown_030046C0.unk07;
            gUnknown_03003100.pos.unk00 = pos.unk00;
            gUnknown_03003100.pos.unk02 = pos.unk02;
            if (gPlaySt.savingEnabled != 0)
                SendActionCommand(0xe, gUnknown_030046C0.unk06, gUnknown_030046C0.unk07, 0);
        }
    }
}
asm(".global sub_080610D0\n.thumb_set sub_080610D0, AiCommitBuild\n");
