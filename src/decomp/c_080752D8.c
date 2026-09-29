#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080752D8.
 * sub_080752D8 @ 0x080752D8
 */

#include "proc.h"
/* MATCHED (wave 38, W38-E), first attempt. Unassigned extra.
 *
 * Broadcasts a new value to ProcScr_WM_MoveScope (sub_0807519C is its
 * body) and to each of the up-to-ten children it has parked in its +0x3c table,
 * skipping the null slots. The proc and child layouts are sub_0807519C's,
 * unchanged -- that function fills the same +0x3c[10] table.
 *
 * `u16 v` and not a bare `(u16)` cast at each use: the ROM narrows ONCE
 * (`lsls #0x10; lsrs #0x10` into the callee-saved r4) and then feeds the same
 * register to both the +0x38 `strh` and the loop's +0x30 `strh`. Two casts
 * would be free at each `strh`, so the single narrowing is the tell for a
 * named local. The parameter stays `int` -- the header declares it that way,
 * and a `u16` parameter would arrive already zero-extended by PROMOTE_MODE with
 * no shift pair at all.
 *
 * The ROM walks the table with a pointer (`adds r0, #0x3c` on the Proc_Find
 * result, then `adds r0, #4`) and lets `proc` die; that is strength_reduce
 * turning `&proc->unk3c[i]` into a giv, and the plain indexed source below is
 * what produces it. The `subs r2, #1; cmp r2, #0; bge` bottom is check_dbra_loop
 * on the ASCENDING loop, not a descending one -- same reading as WorldMapScope_OnEnd.
 */
struct Unk80752D8Child
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u16 unk30;
};
struct Unk80752D8
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ u16 unk38;
    /* 0x3a */ u8 filler_3a[0x2];
    /* 0x3c */ struct Unk80752D8Child *unk3c[10];
};

void SetDifficultyStarsPalette(int a1)
{
    struct Unk80752D8 *proc;
    u16 v;
    int i;

    v = a1;

    proc = Proc_Find(ProcScr_WM_MoveScope);
    proc->unk38 = v;

    for (i = 0; i <= 9; i++)
    {
        if (proc->unk3c[i] != 0)
            proc->unk3c[i]->unk30 = v;
    }
}
asm(".global sub_080752D8\n.thumb_set sub_080752D8, SetDifficultyStarsPalette\n");
