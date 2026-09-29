#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080363D0.
 * sub_080363D0 @ 0x080363D0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* The teardown: release the proc's sprite object and its heap block, then clear
 * this proc's slot in gUnknown_03003124. unk3a is `s8` -- the index is read
 * `ldrsb` -- and the +0x48 read is what fixes FreeMoveSlideGfxBuffer's parameter as the
 * heap pointer (its own definition's comment already says so). */
struct Unk363D0Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ void *unk2c;
    /* 0x30 */ u8 filler_30[0x0a];
    /* 0x3a */ s8 unk3a;
    /* 0x3b */ u8 filler_3b[0x0d];
    /* 0x48 */ void *unk48;
};

void MoveSlide_OnEnd(ProcPtr procArg)
{
    struct Unk363D0Proc *proc = procArg;

    AP_Delete(proc->unk2c);
    FreeMoveSlideGfxBuffer(proc->unk48);
    gUnknown_03003124[proc->unk3a] = 0;
}

asm(".global sub_080363D0\n.thumb_set sub_080363D0, MoveSlide_OnEnd\n");

extern void MoveSlide_Loop(void);

struct ProcCmd CONST_DATA ProcScr_SelectUnit[] =
{
    PROC_ONEND(MoveSlide_OnEnd),
    PROC_REPEAT(MoveSlide_Loop),
    PROC_BLOCK,
};

asm(".global gUnknown_0849BE38\n.set gUnknown_0849BE38, ProcScr_SelectUnit\n");
