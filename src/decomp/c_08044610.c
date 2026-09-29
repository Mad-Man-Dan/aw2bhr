#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044610.
 * sub_08044610 @ 0x08044610
 */

#include "proc.h"
/* MATCHED -- wave 45 (W45-A), 4 attempts plus one permuter run.
 *
 * NEEDS A .rodata POOL WORD PLACED. trymatch reports `relocs: name different
 * symbols that resolve to the same address` at +0x104, original
 * gUnknown_0809138C / candidate .rodata. The promotion must carry
 *     "rodata": ["0x0809138C"]
 * in this function's data/promoted.json entry, then tools/split_rodata.py and
 * tools/gen_lds.py before building.
 *
 * WHY THAT WORD EXISTS, because it looks like a global and is not one: the
 * ROM does `ldr r4, =gUnknown_0809138C; ldr r0, [r4]` -- a TWO-LEVEL read
 * that reads like a pointer variable. The word AT 0x0809138C in baserom.gba
 * is 0x0849A00C, i.e. the address of gUnknown_0849A00C, which THIS SAME
 * FUNCTION also references directly one call later. So it is agbcc's own
 * -fforce-addr address-constant pool, not a variable, and the honest spelling
 * -- naming gUnknown_0849A00C at both call sites -- is what matches. The
 * splitter had invented a gUnknown_ symbol for it; dereferencing the address
 * in baserom.gba is what tells the two apart, and it is worth doing for any
 * two-level read whose pool address lands in a .rodata range.
 *
 * THE RESIDUAL THAT COST THE LAST THREE ATTEMPTS was 2 bytes in the loop
 * PREHEADER, everything else byte-exact:
 *     ROM    mov r6, sl / adds r6, #104 / ... / mov r9, r6
 *     draft  mov r3, r9 / adds r3, #104 / ... / mov sl, r3 / mov r6, sl
 * The draft's &proc->unk68 pseudo was allocated to a HIGH register, so
 * `adds rN, #104` (a low-register form) needed a scratch and then two copies
 * instead of one. Writing `proc->unk68++` INSIDE BOTH ARMS of the inner
 * if/else -- rather than once after it -- is what closed it: it raises that
 * pseudo's reference count enough to win the low callee-saved register under
 *     priority ~ floor_log2(n_refs) * n_refs * freq / live_length
 * (the same model that closed MapEventCond_Army1OwnsEightFixedCells this wave). It costs nothing in
 * code, because gcc's cross-jumping merges the two identical `unk68++; break;`
 * tails back into the single block the ROM has at 0x080447A6 -- which is why
 * the if-arm still reaches it by `b` and not by fall-through.
 *
 * THE PERMUTER FOUND A 99.8% VARIANT AND IT IS THE WRONG ANSWER -- recorded
 * because the failure mode is instructive. It reached score 0 (objdump text
 * identical) by DELETING the increment from the if-arm, leaving it only in the
 * else arm. That is semantically wrong -- the ROM's if-arm branches into the
 * shared increment block -- and the single byte it still differed by, at
 * +0x100, was precisely the branch target that betrayed it. Its other change,
 * an `int new_var = 0;` pressure lever, was tested separately here and is
 * byte-neutral. So the permuter located the right VARIABLE (the increment) by
 * mutating it in a way that could never be shipped; the useful reading of a
 * permuter near-miss is which statement it touched, not the body it produced.
 *
 * OTHER THINGS THE ASSEMBLY DOES NOT HAND YOU:
 *   - the inner test is spelled with `||`, not `&&`. The ROM's unk08-only arm
 *     is the FALL-THROUGH and the ScrollCameraToKeepCellInView arm is forward past the pool;
 *     `if (cell != 0 && (unk01 & 8) == 0)` lays them out the other way round.
 *   - StartUnitSparkleEffect's first two arguments are bound to locals BEFORE the flag
 *     is computed. The ROM loads unk02/unk03 into r3/r4 (clobbering the unit
 *     pointer), then builds the flag in r5, then copies all three into r0-r2.
 *     Naming them at the call site instead loads them straight into r0/r1 and
 *     is 4 bytes short -- the two `adds rN, rM, #0` copies are the tell that
 *     the original had locals there.
 *   - the loop is a `while` with ONE increment at the bottom shared by both
 *     `continue` paths; two separate increments stop gcc merging them into
 *     the ROM's increment-then-test block and make the bottom test re-load
 *     proc->unk68 instead of sign-extending the value it just stored. */

struct Unk08044610Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 filler_66[0x02];
    /* 0x68 */ s16 unk68;
};
void CoPowerUnitEffects_Loop(struct Unk08044610Proc *proc)
{
    struct Unit *unit;
    struct Map *m;
    int idx;
    u8 x;
    u8 y;
    u8 flag;

    proc->unk64++;

    if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
        return;

    if (proc->unk64 <= 4)
        return;

    proc->unk64 = 0;

    while (proc->unk68 <= 0x32)
    {
        unit = &gUnits[gUnknown_03003F2C + proc->unk68];

        if (unit->type != 0
            && gUnknown_084A0090[gPlayers[gUnknown_030033EC].co]
                   .power[gPlayers[gUnknown_030033EC].coActivationMode - 1]
                   .animationCondition(unit) != 0)
        {
            m = gMap;
            idx = m->rowOffset[unit->y] + unit->x;

            if (m->unk234A[idx] == 0 || (unit->flags & 8) != 0)
            {
                gUnknown_084A0090[gPlayers[gUnknown_030033EC].co]
                    .power[gPlayers[gUnknown_030033EC].coActivationMode - 1]
                    .onEachUnit(unit);

                proc->unk68++;
                break;
            }
            else
            {
                ScrollCameraToKeepCellInView(unit->x, unit->y);

                if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
                    return;

                gUnknown_084A0090[gPlayers[gUnknown_030033EC].co]
                    .power[gPlayers[gUnknown_030033EC].coActivationMode - 1]
                    .onEachUnit(unit);

                x = unit->x;
                y = unit->y;
                flag = 0;

                if (gPlayers[gUnknown_030033EC].coActivationMode == 2)
                    flag = 1;

                StartUnitSparkleEffect(x, y, flag);

                proc->unk68++;
                break;
            }
        }

        proc->unk68++;
    }

    if (proc->unk68 == 0x33)
        Proc_Break(proc);

    RebuildMapUnitLayers2();
}
asm(".global sub_08044610\n.thumb_set sub_08044610, CoPowerUnitEffects_Loop\n");
