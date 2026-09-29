#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801906C.
 * sub_0801906C @ 0x0801906C
 */

/*
 * EventOp_SkipUnlessArmyHasCo -- script command: skip a block of nodes unless a CO matches.
 *
 * The current node holds an army index in .unk08, a CO number in .unk0a and a
 * node count in .unk0c. DivRem reduces the CO number modulo 24; if the army is
 * not playing that CO, the cursor jumps .unk0c nodes forward and skips that
 * block of the script. Either way the cursor then steps one more node on, past
 * this command itself, and TRUE comes back so the dispatcher runs the next
 * command in the same frame.
 *
 * Why the C looks odd: there is no pointer local for the cursor -- the skip
 * loop steps `gUnknown_0200C528[a].unk04` itself. That is what lets the
 * compiler keep the cursor in a register for the loop and store it back once on
 * the way out, and it is also why nothing is stored at all when the count is 0.
 * A pointer local written back afterwards is 8 bytes longer. The `(s16)` casts
 * on the two u16 members fold the narrowing into the load and are not evidence
 * that the members are signed.
 */
bool8 EventOp_SkipUnlessArmyHasCo(s16 a)
{
    int army;
    int n;
    int r;

    army = (s16)gUnknown_0200C528[a].unk04->unk08;
    r = DivRem((s16)gUnknown_0200C528[a].unk04->unk0a, 0x18);
    n = gUnknown_0200C528[a].unk04->unk0c;
    if (gPlayers[army].co != r)
    {
        while (n > 0)
        {
            gUnknown_0200C528[a].unk04++;
            n--;
        }
    }
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_0801906C\n.thumb_set sub_0801906C, EventOp_SkipUnlessArmyHasCo\n");
