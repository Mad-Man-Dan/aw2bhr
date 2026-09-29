#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080293C8.
 * sub_080293C8 @ 0x080293C8, sub_08029490 @ 0x08029490
 */

#include "hardware.h"
#include "proc.h"
/* gUnknown_08090B68 is an agbcc -fforce-addr address word: the ROM holds
 * 0x03002EE0 there, which aw2bhr.map names `gpKeySt`. Naming gpKeySt directly
 * reproduces the three-level read. */
/* WAVE 35: CANONICAL `struct Map`. Eight drafts across blocks 0x08029-0x0802B
 * each invented their own body for this tag, with 2 to 7 named fields. Every
 * one compiles and byte-matches ALONE, so trymatch cannot see the problem;
 * merged into a promoted unit it is a hard `redefinition of struct Map`.
 * All seven distinct bodies describe the SAME layout at different
 * resolutions -- the offsets reconcile exactly -- so this is their union, the
 * most refined reading of each region. Byte-neutral: only a field's START
 * OFFSET enters the address arithmetic, never its declared length, and no
 * draft referenced any filler. Keep the drafts in sync; sync_work.py
 * reintroduces whatever the drafts say. */

void TargetPickCursor_Loop(ProcPtr proc)
{
    HandleMoveMapCursor();
    HandleMoveMapCursorInMoveRange();
    HandleMoveCameraWithMapCursor(4);

    if (sub_08029490(proc))
    {
        StepMapCursorAndDraw(2);

        if (gpKeySt->pressed & 1)
        {
            Proc_End(proc);
            gUnknown_03003F40 = gMap->unitUnk[
                gMap->rowOffset[gUnknown_030033E4.unk02]
                + gUnknown_030033E4.unk00];
            sub_08028EE4();
            return;
        }
    }
    else
    {
        StepMapCursorAndDraw(1);
    }

    if (gpKeySt->pressed & 2)
    {
        gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
        gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;
        gUnknown_03003F40 = -1;
        Proc_End(proc);
        sub_08028EE4();
    }
}
asm(".global sub_080293C8\n.thumb_set sub_080293C8, TargetPickCursor_Loop\n");

bool8 sub_08029490(ProcPtr proc)
{
    int idx;

    idx = gMap->rowOffset[gUnknown_030033E4.unk02]
        + gUnknown_030033E4.unk00;

    if (gMap->move[idx] >= 0)
    {
        if (gMap->visible[idx] == 0)
            return 1;

        if (gMap->unit[idx] != 0
            && IsUnitOnCurrentTeam(gMap->unit[idx]))
            return 1;
    }

    return 0;
}
