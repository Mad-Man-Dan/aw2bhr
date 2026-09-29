#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802925C.
 * sub_0802925C @ 0x0802925C
 */

#include "proc.h"
#include "hardware.h"
struct Unk2925CProc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

void TargetPickList_Loop(struct Unk2925CProc *proc)
{
    struct Unk03003338 *p;
    s16 old;
    s8 r;

    r = FindSlotScript((s32)gUnknown_0849A00C);

    if (r != -1)
        return;

    if (gpKeySt->pressed & 2)
    {
        gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
        gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;
        gUnknown_03003F40 = r;
        Proc_End(proc);
        sub_08028EE4();
        ScrollCameraToKeepCellInView(((struct Unk802C57CS *)&gUnknown_030033E4)->unk00,
                     ((struct Unk802C57CS *)&gUnknown_030033E4)->unk02);
        return;
    }

    if (gpKeySt->pressed & 1)
    {
        gUnknown_03003F40 = proc->unk64;
        Proc_End(proc);
        sub_08028EE4();
        return;
    }

    old = proc->unk64;

    if (gpKeySt->repeated & (DPAD_RIGHT | DPAD_DOWN))
    {
        proc->unk64 = proc->unk64 + 1;

        if (proc->unk64 == proc->unk66)
            proc->unk64 = 0;
    }

    if (gpKeySt->repeated & (DPAD_LEFT | DPAD_UP))
    {
        if (proc->unk64 <= 0)
            proc->unk64 = proc->unk66;

        proc->unk64--;
    }

    if (old != proc->unk64)
        PlayMusicOrSfx2(0x77);

    p = GetAttackTargetRecord(proc->unk64);

    gUnknown_030033E4.unk00 = p->unk04;
    gUnknown_030033E4.unk02 = p->unk06;

    ScrollCameraToKeepCellInView(*(s16 *)&p->unk04, *(s16 *)&p->unk06);

    if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
    {
        ResetDisplayEffects();
        return;
    }

    StepMapCursorAndDraw(2);
    RefreshMapCursorInfoPanel();
}
asm(".global sub_0802925C\n.thumb_set sub_0802925C, TargetPickList_Loop\n");
