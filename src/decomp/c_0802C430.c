#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C430.
 * sub_0802C430 @ 0x0802C430, sub_0802C450 @ 0x0802C450
 */

/* Four teardown calls and a counter reset. Same non-Proc object as
 * sub_0802C390 and sub_0802C450 -- +0x1e is inside PROC_HEADER's proc_next, so
 * the prefix is filler. The pointer survives four calls in r4, which is the
 * only reason this function saves a register at all.
 */

struct Unk2C430Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};
#include "hardware.h"
/* sub_0802C390's countdown twin on the same non-Proc object: publish
 * proc->unk1e to the blend coefficient, and either finish or decrement.
 *
 * As in sub_0802C390 the member is read three times with no local binding it.
 * That is what puts the `ldr` of &gUnknown_03001FFC ahead of the `ldrh`, and
 * what makes the `- 1` come off the raw halfword in r1 rather than off a
 * sign-extended copy.
 *
 * `lsls #0x10; cmp #0` with no `asrs` between them is the s16 `== 0` test: only
 * the low 16 bits decide equality, so agbcc drops the sign extension it would
 * need for the ordered compare sub_0802C390 does. A u16 member is byte-
 * identical here and is NOT proved by this function -- the signedness comes
 * from sub_0802C390's arithmetic `asrs #0x11` on the same field.
 *
 * gUnknown_03001FFC is volatile, so the store is not forwarded into the
 * compare; see the note in include/hardware.h.
 */

struct Unk2C450Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

void sub_0802C430(struct Unk2C430Proc *proc)
{
    RebuildMapUnitLayers2();
    ClearBg0Tilemap();
    BG_EnableSyncBG0();
    HideMapPreview();
    proc->unk1e = 6;
}

void sub_0802C450(struct Unk2C450Proc *proc)
{
    gUnknown_03001FFC = proc->unk1e;

    if (proc->unk1e == 0)
    {
        SetDefaultColorEffects();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
    else
    {
        proc->unk1e = proc->unk1e - 1;
    }
}
