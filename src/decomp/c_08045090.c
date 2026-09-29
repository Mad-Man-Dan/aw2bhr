#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045090.
 * sub_08045090 @ 0x08045090
 */

#include "proc.h"
/* Every fifth call (unk2b is a frame divider), advance to the next live army
 * whose gPlayers unk2a differs from the current one's, then walk that
 * army's 50 gUnits slots from unk29 looking for the first live,
 * unflagged unit; decay its unk04_0 by `unk2d * 10` with a floor of 1, start
 * the 0x080452C0 popup for it and stop. unk29 is the resume cursor and wraps
 * to 1 with unk2a++ once it passes 0x32.
 *
 * `i` is u8 and `j` is u16 -- both read off the truncation on their own
 * increments, not off the u8 fields they are seeded from.
 *
 * The army search must be `while (1) { ...; i++; }` and NOT `for (i = ...;;
 * i++)`. Both are the same C, but the `for` puts the increment in its own
 * continue block ahead of the test and enters the loop with a `b` past it,
 * while the ROM's back edge is an unconditional `b` from an increment that
 * sits at the BOTTOM of the body. Costs no bytes, moves eight instructions.
 *
 * gUnknown_084995FE is a declared `s16 []` and the ROM reads it with a plain
 * `ldrh`, so the source has a `(u16)` cast on the entry -- without it agbcc
 * emits `movs r1,#0; ldrsh r0,[r0,r1]`, the same size and a different opcode.
 * Same finding as work/ArmyDefeat_Loop.
 *
 * `proc->unk2a` is re-read from memory inside the second loop rather than
 * reusing `i`, and `proc->unk2d * 10` is NAMED TWICE so it becomes one CSE
 * feeding both arms of the floor test. The `& 0x7f` in the else arm is the
 * bitfield store's own truncation; the constant-1 arm needs none. */
struct Unk45090Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ u8 unk29;
    /* 2a */ u8 unk2a;
    /* 2b */ u8 unk2b;
    /* 2c */ u8 unk2c;
    /* 2d */ u8 unk2d;
};

void CoPowerDamageHeal_DamageLoopSimple(struct Unk45090Proc *proc)
{
    u8 i;
    u16 j;
    struct Unit *unit;

    proc->unk2b++;

    if (proc->unk2b <= 4)
        return;

    proc->unk2b = 0;

    i = proc->unk2a;

    while (1)
    {
        if (i > 4)
        {
            Proc_Break(proc);
            return;
        }

        if (IsPlayerAliveAndActive(i))
        {
            if (gPlayers[i].team != gPlayers[proc->unk2c].team)
                break;
        }

        i++;
    }

    proc->unk2a = i;

    for (j = proc->unk29; j <= 0x32; j++)
    {
        unit = &gUnits[(u16)gUnknown_084995FE[proc->unk2a] + j];

        if (unit->type == 0)
            continue;

        if (unit->flags & 8)
            continue;

        if (unit->hp < proc->unk2d * 10 + 1)
            unit->hp = 1;
        else
            unit->hp = unit->hp - proc->unk2d * 10;

        StartCoPowerUnitSparkle(unit->x, unit->y, proc->unk2c);
        break;
    }

    proc->unk29 = j + 1;

    if (proc->unk29 > 0x32)
    {
        proc->unk29 = 1;
        proc->unk2a++;
    }
}
asm(".global sub_08045090\n.thumb_set sub_08045090, CoPowerDamageHeal_DamageLoopSimple\n");
