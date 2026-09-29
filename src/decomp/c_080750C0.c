#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080750C0.
 * sub_080750C0 @ 0x080750C0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* The body of the ProcScr_WM_DrawDifficultyStars child StartDifficultyStar starts. The scale
 * denominator is a single Interpolate ramp 8 -> 0x100 over 10 frames, and the
 * affine block is c_0807662C.c's zero-angle shape with the roles swapped: pa/pc
 * take the ramp and pb/pd the fixed 0x100. 0x100 is ONE constant in the source
 * -- it is Interpolate's third argument, both fixed denominators and the
 * termination test -- which is why the ROM parks it in sb across all six calls
 * instead of rebuilding it, the same reading c_08075C98.c gives its r8. */
struct Unk80750C0
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ u16 unk38;
    /* 0x3a */ u16 unk3a;
};

void DifficultyStars_PopInLoop(struct Unk80750C0 *proc)
{
    int t = Interpolate(4, 8, 0x100, proc->unk3a, 10);

    SetObjAffine(0xC,
                 Div(COS_Q12(0) << 4, t),
                 Div(-SIN_Q12(0) << 4, 0x100),
                 Div(SIN_Q12(0) << 4, t),
                 Div(COS_Q12(0) << 4, 0x100));

    PutSpriteExt(1, (s16)(proc->unk34 | 0x1800), (s16)(proc->unk36 | 0x100),
                 gUnknown_081CC4F0, gUnknown_0861433C[proc->unk38]);

    if (t == 0x100)
    {
        proc->unk3a = 0;
        Proc_Break(proc);
    }

    proc->unk3a++;
}

asm(".global sub_080750C0\n.thumb_set sub_080750C0, DifficultyStars_PopInLoop\n");
