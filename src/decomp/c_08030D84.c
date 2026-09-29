#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030D84.
 * sub_08030D84 @ 0x08030D84, sub_08030DEC @ 0x08030DEC, sub_08030E40 @ 0x08030E40
 */

/* MATCHED (wave 42, W42-K). Needs its .rodata pool word placed:
 *   "rodata": ["0x08090CEC"]
 * -- an agbcc -fforce-addr word holding &gUnknown_0849B018, not a global.
 *
 * The condition is spelled `<= 0xf` and NOT `> 0xf`: the ROM's `bhi` jumps
 * to the packet-send block, so the increment is the fall-through and is
 * therefore the THEN arm. Writing it the other way round puts the big block
 * first and moves the `b` onto the wrong side.
 *
 * unk1e is read three times at the top -- once for the compare, once for the
 * increment, and once dead in front of the `strb`. Only the dead one is the
 * volatile tell; the other two are two separate reads in the source.
 *
 * gUnknown_020256DA is its own symbol rather than a member of
 * gUnknown_020256D4: the ROM builds the two addresses from two independent
 * pool words, in first-reference order (payload first), where one base plus
 * a displacement would have emitted a single word. */

void LinkSendHelloPacket(void)
{
    int i;

    if (gUnknown_0849B018->unk1e <= 0xf)
    {
        gUnknown_0849B018->unk1e++;
    }
    else
    {
        for (i = 0; i <= 0xc; i++)
            gUnknown_020256DA[i] = gUnknown_0849B038[i];

        gUnknown_020256D4.unk00 = 0xad;
        gUnknown_020256D4.unk01 = gUnknown_0849B018->unk06;
        gUnknown_020256D4.unk02 = gUnknown_0849B018->unk00;
        sub_0802F588(&gUnknown_020256D4, 0x1a);
        gUnknown_0849B018->unk1e = 0;
    }
}
asm(".global sub_08030D84\n.thumb_set sub_08030D84, LinkSendHelloPacket\n");

/* MATCHED (wave 42, W42-K), byte-for-byte with `relocs: match` -- no pool
 * word to place. gUnknown_0849B018 is referenced ONCE here and a single
 * reference gets the direct `ldr =gUnknown_0849B018`; its neighbours in this
 * block reference it four or more times across a loop and so do get an
 * -fforce-addr word. That contrast is the cleanest example of the rule in
 * the block.
 *
 * IT IS AN IF/ELSE WITH TWO STORES, NOT A `?:` ON ONE STORE -- and the store
 * itself cannot tell you that, because cross-jumping merges the two `strb`s
 * into one at the join either way. What discriminates is the DUPLICATED
 * ADDRESS: the ROM computes `adds r2,r4,#1; adds r1,r2,r5` inside BOTH arms,
 * after the call. The `?:` spelling hoists that computation ahead of the
 * `bl` instead, which costs two more callee-saved registers
 * (`push {r4,r5,r6,r7,lr}` against the ROM's `push {r4,r5,lr}`) because the
 * address then has to stay live across the call. Measured both ways this
 * wave. This does not contradict the standing rule that `?:` on a store is
 * real code the original wrote -- it just is not what this one wrote.
 *
 * `int i`, not s8: the `lsl #0x18; asr #0x18` in front of the `bl` is
 * SioIsPlayerLinked's own s8 parameter, not a cast in the source, and the loop
 * increment carries no narrowing at all.
 *
 * The tail is unk38[unk06 + 1] -- a variable index on a 0x38 base, biased by
 * one into the 1..4 player-slot convention the rest of the record uses. */

void LinkAssignArmyControllers(void)
{
    int i;

    for (i = 0; i <= 3; i++)
    {
        if (SioIsPlayerLinked(i) == 1)
            gUnknown_03003FF8[i + 1] = i + 3;
        else
            gUnknown_03003FF8[i + 1] = 0;
    }

    gPlaySt.aiControlled[gUnknown_0849B018->unk06 + 1] = 1;
}
asm(".global sub_08030DEC\n.thumb_set sub_08030DEC, LinkAssignArmyControllers\n");

/* MATCHED (wave 42, W42-K). Needs its .rodata pool word placed:
 *   "rodata": ["0x08090CF0"]
 * -- an agbcc -fforce-addr word holding &gUnknown_0849B018, not a global.
 * This is the TOP word of that run: baserom.gba has 0x0849B018 at every
 * word from 0x08090CA4 up to 0x08090CF0, and 0x08090CF4 holds 0x0CA50CA4,
 * which is not an address and is where the run ends.
 *
 * `unk1ab2 |= r << i` shifts the RETURNED VALUE, not a literal 1. Inside the
 * `r == 1` arm gcc knows the register already holds 1, so it emits
 * `adds r1,r3,#0` -- a copy of the result -- where `1 << i` emits
 * `movs r1,#1`. Same size, different register, and the ROM shows the copy.
 *
 * Every store here carries a dead load of its own address in front of it.
 * That is the volatile on unk1ab0/unk1ab2/unk1ab3/unk1ab4 doing its work and
 * none of it is authored; do not try to spell those loads. */

void LinkRecordSessionInfo(void)
{
    int i;
    bool8 r;

    gUnknown_0849B018->unk1ab4 = gUnknown_0849B018->unk06;
    gUnknown_0849B018->unk1ab3 = 0;
    gUnknown_0849B018->unk1ab2 = 0;

    for (i = 0; i <= 3; i++)
    {
        r = SioIsPlayerLinked(i);

        if (r == 1)
        {
            gUnknown_0849B018->unk1ab3++;
            gUnknown_0849B018->unk1ab2 |= r << i;
        }
    }

    gUnknown_0849B018->unk1ab0 = gGameClock;
}
asm(".global sub_08030E40\n.thumb_set sub_08030E40, LinkRecordSessionInfo\n");
