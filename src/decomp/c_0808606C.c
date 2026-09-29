#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808606C.
 * sub_0808606C @ 0x0808606C
 */

/* A three-state step driven off the proc's own +0x30 phase counter: state 0 is
 * idle, state 1 scrolls one way or the other depending on the sign of the s16
 * at +0x4e, state 3 stops and rewinds the phase to 0, and every other non-zero
 * state just advances it.
 *
 * The parameter is the proc struct itself and NOT `ProcPtr` with a typed local:
 * the ROM keeps the object in one register for the whole function, where the
 * local copy stays live across both calls, fails to coalesce and splits the
 * shared `str r0, [r4, #0x30]` tail across two registers. See the note in
 * include/unknown-functions.h.
 *
 * The `ldr r0, [r4, #0x30]` after the state-1 arm is the reload `proc->unk30++`
 * needs once the call has clobbered r0; the default arm reaches the same
 * `adds r0, #1; str` with the value still live, and state 3's `movs r0, #0`
 * reaches the `str` alone. Both are jump.c cross-jumps, not source structure. */

struct Unk8606CProc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x1a];
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x0c];
    /* 0x5c */ int unk5c;
};

void MapSelectList_StepScrollRedraw(struct Unk8606CProc *proc)
{
    if (proc->unk30 == 0)
        return;

    if (proc->unk30 == 1)
    {
        if (proc->unk4e < 0)
            sub_08086A58(proc->unk5c, gUnknown_03005928 + 1, -1);
        else
            sub_08086A58(proc->unk5c - 1, gUnknown_03005928 + 1, -1);
        proc->unk30++;
    }
    else if (proc->unk30 == 3)
    {
        sub_08086A58(proc->unk5c, gUnknown_03005928, 0);
        proc->unk30 = 0;
    }
    else
    {
        proc->unk30++;
    }
}
asm(".global sub_0808606C\n.thumb_set sub_0808606C, MapSelectList_StepScrollRedraw\n");
