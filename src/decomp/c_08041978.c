#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041978.
 * sub_08041978 @ 0x08041978
 */

#include "proc.h"
/* MATCHED (wave 66, W66-B), 968/968 bytes. The two remaining axes closed with
 * `gUnknown_03003F50 = (r = GetBattleSceneAttackerSide(...))`, which materializes the store
 * address before the call while preserving its return in r0 for `1 - r`, and
 * block-local `q`/`diff` temporaries, which put `t - 1` after Div without
 * fold reassociating it to `t - (q + 1)`. Promotion needs .rodata words
 * 0x08091340, 0x08091344, 0x08091348, 0x0809134C and 0x08091350.
 *
 * Historical wave-57 diagnosis: size is EXACT (968 bytes), the whole
 * function is transcribed, and the residual is THREE SPOTS, all size-neutral.
 * Nothing structural is open -- do not re-derive the shape.
 *
 * ---- WHAT IS ALREADY SETTLED ----
 *
 * The magic ladder (`* 0x55555555; rsbs; asrs #8`) is the pointer-difference
 * chapter of docs/agbcc-codegen.md: post_shift 8 and the inverse of 3 means a
 * 768-byte stride, i.e. 64 struct Unit records = one army, and the `+ 1`
 * makes it a 1-based gPlayers index. struct Unk41978Army exists purely
 * for that size.
 *
 * The head's pointer is gUnknown_030040D8 CAST TO struct Unit *, NOT
 * `gBattleAttacker->unit`. Both are the same runtime
 * value and both compile, but only gUnknown_030040D8 puts the five force-addr
 * words in the ROM's slots -- and it is what makes the two halves symmetric:
 * `GetUnitCostWithCoBonus(army1, gUnknown_030040D8->unk00)` against
 * `GetUnitCostWithCoBonus(army2, unit->type)`. The cast is the merge
 * include/unknown-globals.h already records for gUnknown_08091364, which is
 * typed `struct Unit **` for exactly this reason. Spelling the head
 * through gUnknown_030013D0 instead cost 8 bytes and two pool words.
 *
 * gUnknown_03004580's rows are a PLAIN u16 ARRAY, so `g[i][k]` is right and a
 * local row struct is wrong -- the ROM hoists every column onto the base
 * (`adds r2, r4, #2` then `+ i*16`) instead of folding it into the store
 * displacement. docs/agbcc-codegen.md's column-offset chapter names this exact
 * global as its array-row example. gUnknown_030044B0 is the opposite case: the
 * `+8` there IS a load displacement, so it needs the local struct.
 *
 * `sel` must be bound INSIDE the [1] store's subscript, not as its own
 * statement -- as a statement it puts `mov sl, r0` ahead of the row-address
 * computation, where the ROM interleaves them.
 *
 * The five `.rodata` words are this unit's -fforce-addr pool and they land
 * exactly on the ROM's, verified positionally by trymatch:
 *     .rodata+0x00 -> 0x08091340   &gUnknown_030040D8
 *     .rodata+0x04 -> 0x08091344   &gPlayers
 *     .rodata+0x08 -> 0x08091348   &gUnknown_030013D0
 *     .rodata+0x0c -> 0x0809134C   &gUnknown_030013B0
 *     .rodata+0x10 -> 0x08091350   &gPlaySt
 * The last one agrees with the block listing already in
 * docs/agbcc-codegen.md, which is an independent check on the other four.
 * These are the documented false-mismatch class; the promotion must carry them.
 *
 * ---- THE REMAINING DIFF, EXACTLY ----
 *
 * 1. `t - 1 - Div(...)` at BOTH x1 and x2 (2 sites, 3 instructions each way):
 *      ROM   subs r1, r4, #1 ; subs r1, r1, r0 ; adds r6, r1, #0
 *      here  adds r0, #1     ; subs r0, r4, r0 ; adds r6, r0, #0
 *    i.e. the ROM keeps `(t - 1) - q` and this draft is folded to
 *    `t - (q + 1)`. RULED OUT: parenthesising, `(t - 1) - q` spelled out, and
 *    swapping the multiply to `expr * c` (that last one WAS needed and is in --
 *    it fixed the `muls` operand order and the copy that feeds it). NOT tried:
 *    binding `t - 1` to its own local, or `--t`; both move the subtraction
 *    ahead of the `bl Div`, which the ROM does not do, so they need measuring
 *    rather than assuming.
 *
 * 2. `ldr r5, =gUnknown_03003F50` lands AFTER `bl GetBattleSceneAttackerSide` here and
 *    BEFORE it in the ROM (1 instruction, size-neutral). The ROM keeps the
 *    address live across the call. `gUnknown_03003F50 = GetBattleSceneAttackerSide(...)` as a
 *    single statement is the obvious lever (expand_assignment materialises the
 *    LHS address first) but it costs the `r` local, and `1 - r` must stay on
 *    the CALL'S RETURN -- the ROM does `movs r1,#1; subs r1,r1,r0` with no
 *    re-read, so re-reading gUnknown_03003F50 there would be a new diff. NOT
 *    tried; this is where the next attempt should go.
 *
 * 3. Everything else in the report is `.rodata` reloc naming (item above).
 *
 * Three try_match attempts spent: 54.6% -> size-exact-but-8-short -> 97.5%. */

struct Unk41978Army
{
    struct Unit unk00[64];
};
struct Unk41978Cmd
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
};
struct Unk41978Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
};
/* Promoted but undeclared; signatures taken from src/decomp/c_08041D40.c and
 * src/decomp/c_080440A8.c, not derived. */
int GetBattleSceneAttackerSide(struct Unit *, struct Unit *);
void AddCoPowerCharge(int, int);

void StartUnitAttack(u8 a1, int a2)
{
    struct Unit *unit;
    struct Unit *sel;
    u8 b;
    u8 army1;
    u8 army2;
    int r;
    int t;
    int c;
    int x1;
    int x2;
    int v;

    b = a2;

    unit = &gUnits[a1];

    army1 = (struct Unk41978Army *)(struct Unit *)gUnknown_030040D8
          - (struct Unk41978Army *)gUnits + 1;
    army2 = (struct Unk41978Army *)unit - (struct Unk41978Army *)gUnits + 1;

    if (a1 == 0)
    {
        ((struct Unit *)gUnknown_030040D8)->ammo--;
        LockUnitSelection();
        CommitUnitMove();
    }
    else
    {
        ((struct Unk41978Cmd *)gUnknown_030044B0)->unk08 = gUnknown_03001FD4;
        CalcAttackOutcome(gUnknown_03003F38, a1, &gUnknown_03003100.pos);

        gUnknown_03003F50 =
            (r = GetBattleSceneAttackerSide((struct Unit *)gUnknown_030040D8, unit));
        gUnknown_03004484 = 1 - r;
        gUnknown_0300450C = gUnknown_03003F50;

        gUnknown_03004580[gUnknown_03003F50][0] = gPlayers[army1].teamColor - 1;
        gUnknown_03004580[gUnknown_03004484][0] = gPlayers[army2].teamColor - 1;

        gUnknown_03004580[gUnknown_03003F50][1] =
            gUnknown_0809131E[(sel = (struct Unit *)gUnknown_030040D8)->type];
        gUnknown_03004580[gUnknown_03004484][1] = gUnknown_0809131E[unit->type];

        gUnknown_03004580[gUnknown_03003F50][2] =
            gUnknown_08091318[gBattleAttacker->attackType];
        gUnknown_03004580[gUnknown_03004484][2] =
            gUnknown_08091318[gBattleDefender->attackType];

        gUnknown_03004580[gUnknown_03003F50][3] = gBattleAttacker->terrainId;
        gUnknown_03004580[gUnknown_03004484][3] = gBattleDefender->terrainId;

        gUnknown_03004580[gUnknown_03003F50][4] =
            gPlayers[(struct Unk41978Army *)sel
                              - (struct Unk41978Army *)gUnits + 1].co;
        gUnknown_03004580[gUnknown_03004484][4] =
            gPlayers[(struct Unk41978Army *)unit
                              - (struct Unk41978Army *)gUnits + 1].co;

        gUnknown_03004580[gUnknown_03003F50][5] = sel->hp;
        gUnknown_03004580[gUnknown_03004484][5] = unit->hp;

        gUnknown_03004580[gUnknown_03003F50][6] = gBattleAttacker->remainingHp;
        gUnknown_03004580[gUnknown_03004484][6] = gBattleDefender->remainingHp;

        gUnknown_03004580[gUnknown_03003F50][7] = gBattleAttacker->terrainDefense;
        gUnknown_03004580[gUnknown_03004484][7] = gBattleDefender->terrainDefense;

        gUnknown_03004528[gUnknown_03003F50] = (u8 *)sel;
        gUnknown_03004528[gUnknown_03004484] = (u8 *)unit;

        gUnknown_03004520 = gPlaySt.weather;

        if (sel->hp != 0)
            t = Div(sel->hp - 1, 10) + 1;
        else
            t = 0;

        c = GetUnitCostWithCoBonus(army1, gUnknown_030040D8->unk00);

        if (gBattleAttacker->remainingHp != 0)
        {
            int q;
            int diff;
            q = Div(gBattleAttacker->remainingHp - 1, 10);
            diff = t - 1;
            diff -= q;
            x1 = diff * c;
        }
        else
            x1 = t * c;

        if (unit->hp != 0)
            t = Div(unit->hp - 1, 10) + 1;
        else
            t = 0;

        c = GetUnitCostWithCoBonus(army2, unit->type);

        if (gBattleDefender->remainingHp != 0)
        {
            int q;
            int diff;
            q = Div(gBattleDefender->remainingHp - 1, 10);
            diff = t - 1;
            diff -= q;
            x2 = diff * c;
        }
        else
            x2 = t * c;

        AddCoPowerCharge(army1, x1 + Div(x2, 2));
        AddCoPowerCharge(army2, x2 + Div(x1, 2));

        v = gPlaySt.animOpts;

        if (v == 3)
            v = (gPlayers[gUnknown_030033EC].aiControlled == 1) ? 2 : 0;

        if (v != 0)
        {
            ResetDisplayEffects();
            ((struct Unk41978Proc *)Proc_Start(gUnknown_0849FEF8, PROC_TREE_3))->unk64 = b;
        }
        else
        {
            ResetDisplayEffects();
            ((struct Unk41978Proc *)Proc_Start(gUnknown_0849FFB0, PROC_TREE_3))->unk64 = b;
        }
    }
}
asm(".global sub_08041978\n.thumb_set sub_08041978, StartUnitAttack\n");
