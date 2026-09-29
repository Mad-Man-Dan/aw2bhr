#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060AB0.
 * sub_08060AB0 @ 0x08060AB0
 */

/* MATCHED (wave 60, W60-E), 668 bytes, size exact.
 *
 * PROMOTION NOTE: trymatch reports `relocs: name different symbols that resolve
 * to the same address` at +0x028 and +0x1d0 -- the documented false mismatch --
 * and this function's data/promoted.json entry needs
 *     "rodata": ["0x0816DAD8", "0x0816DADC"]
 * before tools/split_rodata.py and tools/gen_lds.py are re-run.
 *
 * The AI's unit-production pick. Loop A finds the type class with the highest
 * remaining demand in the volatile gUnknown_02029C20 table, loop B scores every
 * buildable type against that class through three GetCoAdjustedBaseDamage probes, loop C
 * strikes out types whose demand is below their gUnknown_03004640 score, and
 * loop D picks the best survivor and buys it if the army can afford it and the
 * purchase stays under the two gUnknown_085766E0 ceilings.
 *
 * THE TWO RETRIES ARE `goto`s, NOT LOOP STATEMENTS, and that is the whole shape
 * of the function rather than a stylistic choice. Written as `for (;;)` with
 * `continue`/`break` the two back edges become NOTE_INSN_LOOP_BEG loops, and
 * loop.c then hoists `&TAB->v[pick]` and `&gUnknown_085D5ABC` into a preheader
 * the ROM does not have -- worth a spilled 4-byte frame slot, a `b` into a
 * rotated loop top, and the LOSS of the -fforce-addr word at 0x0816DADC, since
 * -fforce-addr and LICM are the same decision (see the W51-J chapter in
 * docs/agbcc-codegen.md). `goto` emits no loop notes, so nothing is hoisted and
 * the address constants are materialised where the ROM materialises them.
 * Measured: the `for (;;)` spelling is `sub sp, #0x2c`, this one is `#0x28`.
 *
 * `x` IS ONE VARIABLE shared by loop A's running maximum and loop D's, and the
 * live ranges overlap across the retry edge: `movs r7,#0` appears exactly twice,
 * at function entry and at loop D, and a `goto retry` re-enters loop A with
 * whatever loop D left behind. The entry one is the DECLARATION INITIALISER --
 * that is why it sits ahead of the gUnknown_030046C0.unk06 test rather than
 * after it.
 *
 * The `? :` on GetCoAdjustedBaseDamage's fifth argument is NOT a `? :`. The ROM duplicates
 * the whole gPlayers subscript and both `ldrb`s into each arm and
 * cross-jumps only from `str r2,[sp]` onward, which an argument-position
 * COND_EXPR cannot produce -- gcc evaluates that argument into one pseudo and
 * the other arguments outside the branch. Two full calls in an if/else is what
 * gives it, the same reading src/decomp/c_08038C98.c records for its own pair.
 *
 * `gUnknown_02029C20` is declared `extern u8 []` because src/decomp/c_0806171C.c
 * needs `gUnknown_02029C20 + 0x34` in bytes; it is really a volatile u16 table
 * indexed 1..24. The file-local view struct is mandatory and a `volatile u16 *`
 * cast is NOT a substitute -- the dead `ldrh` in front of each `strh` needs a
 * COMPONENT_REF root (the W50-A / W56-I chapters in docs/agbcc-codegen.md).
 * AiCalcEnemyMassNetOfOwnCounters reaches the same symbol the same way.
 *
 * The pool words at 0x0816DAD8 and 0x0816DADC are agbcc's own -fforce-addr
 * address constants for gUnknown_030046C0 and gUnknown_085D5ABC, verified
 * against baserom.gba by W49-E; the honest spelling emits both, and the SECOND
 * reference to each (gUnknown_030046C0 at the success store, gUnknown_085D5ABC
 * at the unk1b test) comes out as an ordinary inline pool word, exactly as in
 * the ROM.
 *
 * `cost` is `x*5` then `lsls #0x11; lsrs #0x10` -- a net left shift of one under
 * a u16 truncation, i.e. `(u16)(x * 10)`, authored as a multiply into a `u16`
 * local so shorten_binary_op folds it into the ROM's two instructions.
 *
 * Loop C's `subs r5,#1; cmp r5,#0; bge` is check_dbra_loop on the ASCENDING
 * source loop below, and `movs r6,#0` before it is LICM hoisting the store's
 * constant -- neither is authored.
 *
 * Spill slots follow declaration order (W50-A): `thr` at [sp,#0x1c] then `v` at
 * [sp,#0x20], so those two declarations must stay in that order.
 *
 * struct Unk085766E0's unk21 and unk23 were carved for this function; see the
 * comments beside them in include/unknown-globals.h, which also record that
 * they ALIAS the 12-byte row table this same pointer carries at +0x14. */

struct Unk60AB0Tab
{
    volatile u16 v[0x19];
};
#define TAB ((struct Unk60AB0Tab *)gUnknown_02029C20)
struct Unk60AB0Row
{
    /* 0x00 */ u8 v[7];
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u8 filler_08[4];
};
struct Unk60AB0Tbl
{
    /* 0x00 */ u8 filler_00[0x14];
    /* 0x14 */ struct Unk60AB0Row rows[1];
};

void AiConsiderBuildingCounterUnit(void)
{
    u8 buf[24];
    u8 thr;
    int v;
    s16 x = 0;
    u8 pick;
    u8 sel;
    u8 r;
    int a;
    int b;
    int k;
    u16 cost;
    u32 funds;
    int i;

    if (gUnknown_030046C0.unk06 != 0)
        return;

    if (gUnknown_03004674 == 0)
        v = 100;
    else
        v = CountUnitsWithTypeTag(4) * 100 / gUnknown_03004674;

    AiCalcBuildPriorities();
    AiCalcEnemyMassNetOfOwnCounters();

retry:
    pick = 0;

    for (i = 1; i <= 24; i++)
    {
        if (x < (s16)TAB->v[i])
        {
            x = TAB->v[i];
            pick = i;
        }
    }

    if (pick == 0)
        return;

    if (x > 100)
        thr = 40;
    else
        thr = x / 3;

    for (i = 1; i <= 24; i++)
    {
        buf[i - 1] = 0;

        if (((struct Unk60AB0Tbl *)gUnknown_085766E0)->rows[i - 1].unk07 != 0)
        {
            a = GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                             gPlayers[gUnknown_030033EC].coMode,
                             i, pick, 0);
            b = GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                             gPlayers[gUnknown_030033EC].coMode,
                             i, pick, 1);

            if (a > b)
                r = GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                                 gPlayers[gUnknown_030033EC].coMode,
                                 i, pick, 0);
            else
                r = GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                                 gPlayers[gUnknown_030033EC].coMode,
                                 i, pick, 1);

            if (r >= thr)
                buf[i - 1] = r;
        }
    }

    for (i = 1; i <= 24; i++)
    {
        if (((struct Unk60AB0Tbl *)gUnknown_085766E0)->rows[i - 1].unk07
                + ((struct Unk60AB0Tbl *)gUnknown_085766E0)->rows[i - 1].unk07 / 2
            < (s16)gUnknown_03004640[i])
            buf[i - 1] = 0;
    }

again:
    sel = 0;
    x = 0;

    for (i = 1; i <= 24; i++)
    {
        if (x < buf[i - 1])
        {
            x = buf[i - 1];
            sel = i;
        }
    }

    if (sel != 0)
    {
        buf[sel - 1] = 0;

        switch (gUnknown_085D5ABC[sel].deployLocation)
        {
        case 0x10:
            k = 4;
            break;
        case 0x20:
            k = 6;
            break;
        default:
            k = 2;
            break;
        }

        cost = GetUnitCostWithCoBonus(gUnknown_030033EC, sel) * 10;

        if (CountBuildablePropertiesOfKind(k) == 0)
            goto again;

        funds = gPlayers[gUnknown_030033EC].funds;

        if (cost > funds)
            goto again;

        if ((gUnknown_085D5ABC[sel].unk1b == 4 && v > gUnknown_085766E0->unk21)
            || cost * 100 / funds > gUnknown_085766E0->unk23)
        {
            TAB->v[pick] = 0;
            goto retry;
        }

        gUnknown_030046C0.unk06 = sel;
        return;
    }

    TAB->v[pick] = sel;
    goto retry;
}
asm(".global sub_08060AB0\n.thumb_set sub_08060AB0, AiConsiderBuildingCounterUnit\n");
