#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044E10.
 * sub_08044E10 @ 0x08044E10, sub_08044F24 @ 0x08044F24
 */

#include "proc.h"
/* MATCHED -- wave 45 (W45-A), first attempt.
 *
 * A repair tick: every 5 frames, while the FindSlotScript slot for
 * gUnknown_0849A00C is free, it scans the current army's units from unk29 and
 * heals the first live, un-flagged one by unk2e * 10, clamped to 100.
 *
 * The `push {r4..r7,lr}` plus second `push {r5,r6,r7}` is a HIGH-register
 * spill (r8/sb/sl), and it is NOT a loop signal. It is the member addresses
 * &unk2b, &unk2c and &unk29 staying live across the calls, plus `proc`
 * itself. They come out of ordinary CSE of the member addresses; nothing
 * about them needs to be authored.
 *
 * The heal is a BITFIELD assignment, not hand-written masking: unk04_0 is the
 * 7-bit field already in include/unknown-globals.h, so `unit->hp = 100`
 * emits `(v & ~0x7f) | 100` with the mask on the value folded away (100 fits
 * in 7 bits) while the other arm keeps its `& 0x7f`. That ASYMMETRY between
 * the two arms is the tell for a bitfield store.
 *
 * `(u16)gUnknown_084995FE[...]` needs the cast: the global is `s16 []` and
 * without it agbcc emits `ldrsh` where the ROM has `ldrh`.
 *
 * The clamp keeps the ROM's own operand order, `unk04_0 > 100 - unk2e * 10`,
 * because `unk2e * 10` is shared with the else arm and materialises once. */

struct Unk08044E10Proc
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 unk2b;
    /* 0x2c */ u8 unk2c;
    /* 0x2d */ u8 unk2d;
    /* 0x2e */ u8 unk2e;
};
/* MATCHED -- wave 45 (W45-A), 2 attempts.
 *
 * The DAMAGE twin of CoPowerDamageHeal_HealLoop: same 5-frame gate, same inner unit scan,
 * same 7-bit unk04_0 bitfield store -- but subtracting unk2d * 10 with a floor
 * of 1 instead of adding unk2e * 10 with a ceiling of 100, plus a halving of
 * unk06_0 when unk30 is set. It is wrapped in an outer scan for the first army
 * 0..4 that is alive (IsPlayerAliveAndActive) and shares the current army's unk2a.
 *
 * TWO THINGS THE 0.746 MNEMONIC SIMILARITY WITH CoPowerDamageHeal_HealLoop DOES NOT GIVE
 * YOU. Both were the attempt-1 miss; both are invisible in the shared shape.
 *
 * 1. THE INNER LOOP INDEXES gUnknown_084995FE[proc->unk2a], NOT unk2c.
 *    CoPowerDamageHeal_HealLoop's reads unk2c. Here the ROM reads it through `sl`, which is
 *    &proc->unk2a -- the army the outer loop just selected and stored --
 *    while `sb` = &proc->unk2c is used only for StartCoPowerUnitSparkle's third
 *    argument. Two different member addresses in two high registers, and
 *    picking the wrong one is byte-identical in shape.
 *
 * 2. THE OUTER LOOP MUST BE `while (1)` WITH THE INCREMENT AT THE BOTTOM, not
 *    a `for` with the bound in the condition. The ROM has ONE `cmp r5, #4` at
 *    the top and an unconditional `b` back to it -- the un-rotated shape.
 *    `for (j = ...; ; j++)` with the guard inside the body does NOT give it:
 *    gcc puts the increment BEFORE the test and adds an entry branch to skip
 *    it, and the giv (`j * 0x3c`, the gPlayers element offset) is
 *    then never strength-reduced, so the multiply moves inside the loop.
 *    Written as `while (1) { if (j > 4) {...return;} ...; j++; }` the exit
 *    test stays at the top, `adds r6, #0x3c` appears in the bottom block
 *    ahead of `adds r5, #1`, and the biv/giv init lands in the preheader.
 *
 *    WHY IT STAYS UN-ROTATED WHEN CoPowerDamageHeal_HealLoop's INNER `for` DOES ROTATE, and
 *    this is the transferable part: stmt.c's `expand_end_loop` only moves a
 *    leading conditional jump to the bottom when that jump targets the loop's
 *    own exit label. Here it targets a `Proc_Break(proc); return;` block
 *    (cross-jumped with the unk2d == 0 early exit at the top of the
 *    function), which is not the exit label, so the rotation never fires.
 *    A LOOP WHOSE NORMAL TERMINATION IS A RETURN RATHER THAN A FALL-THROUGH
 *    IS THE READABLE FORM OF THAT TELL -- and in the ROM it shows up as the
 *    exit branch pointing BACKWARD, to a block above the loop. */

struct Unk08044F24Proc
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 unk2b;
    /* 0x2c */ u8 unk2c;
    /* 0x2d */ u8 unk2d;
    /* 0x2e */ u8 unk2e;
    /* 0x2f */ u8 unk2f;
    /* 0x30 */ u8 unk30;
};

void CoPowerDamageHeal_HealLoop(struct Unk08044E10Proc *proc)
{
    int i;
    struct Unit *unit;

    if (proc->unk2e == 0)
    {
        Proc_Break(proc);
        return;
    }

    proc->unk2b++;

    if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
        return;

    if (proc->unk2b <= 4)
        return;

    proc->unk2b = 0;

    if (proc->unk2c > 4)
    {
        Proc_Break(proc);
        return;
    }

    for (i = proc->unk29; i <= 0x32; i++)
    {
        unit = &gUnits[(u16)gUnknown_084995FE[proc->unk2c] + i];

        if (unit->type == 0)
            continue;

        if (unit->flags & 8)
            continue;

        if (unit->hp > 100 - proc->unk2e * 10)
            unit->hp = 100;
        else
            unit->hp = unit->hp + proc->unk2e * 10;

        RedrawUnitLayer();
        StartCoPowerUnitSparkle(unit->x, unit->y, proc->unk2c);
        break;
    }

    proc->unk29 = i + 1;

    if (proc->unk29 > 0x32)
    {
        Proc_Break(proc);
        proc->unk29 = 1;
        proc->unk2a = 0;
    }
}
asm(".global sub_08044E10\n.thumb_set sub_08044E10, CoPowerDamageHeal_HealLoop\n");

void CoPowerDamageHeal_DamageLoop(struct Unk08044F24Proc *proc)
{
    int i;
    int j;
    struct Unit *unit;

    if (proc->unk2d == 0)
    {
        Proc_Break(proc);
        return;
    }

    proc->unk2b++;

    if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
        return;

    if (proc->unk2b <= 4)
        return;

    proc->unk2b = 0;

    j = proc->unk2a;

    while (1)
    {
        if (j > 4)
        {
            Proc_Break(proc);
            return;
        }

        if (IsPlayerAliveAndActive(j) != 0
            && gPlayers[j].team != gPlayers[proc->unk2c].team)
            break;

        j++;
    }

    proc->unk2a = j;

    for (i = proc->unk29; i <= 0x32; i++)
    {
        unit = &gUnits[(u16)gUnknown_084995FE[proc->unk2a] + i];

        if (unit->type == 0)
            continue;

        if (unit->flags & 8)
            continue;

        if (unit->hp < proc->unk2d * 10 + 1)
            unit->hp = 1;
        else
            unit->hp = unit->hp - proc->unk2d * 10;

        if (proc->unk30 != 0)
            unit->fuel = unit->fuel >> 1;

        RedrawUnitLayer();
        StartCoPowerUnitSparkle(unit->x, unit->y, proc->unk2c);
        break;
    }

    proc->unk29 = i + 1;

    if (proc->unk29 > 0x32)
    {
        proc->unk29 = 1;
        proc->unk2a++;
    }
}
asm(".global sub_08044F24\n.thumb_set sub_08044F24, CoPowerDamageHeal_DamageLoop\n");
