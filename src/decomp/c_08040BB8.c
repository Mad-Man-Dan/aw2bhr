#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040BB8.
 * sub_08040BB8 @ 0x08040BB8
 */

#include "hardware.h"
#include "proc.h"
struct Unk40BB8Proc
{
    /* 00 */ u8 filler_00[0x30];
    /* 30 */ struct Unk0801C210 *unk30;
    /* 34 */ u8 filler_34[0x08];
    /* 3c */ u16 unk3c;
    /* 3e */ s16 unk3e;
    /* 40 */ u8 filler_40[0x0c];
    /* 4c */ s16 unk4c;
};

void CaptureAnimPopup_Loop(struct Unk40BB8Proc *proc)
{
    int t = Interpolate(1, 0, 0x100, proc->unk4c, 10);
    int xv = proc->unk3c & 0x1FF;
    int yv = (proc->unk3e - 0x10) & 0xFF;

    AP_Update(proc->unk30, xv, yv | 0x100);

    SetObjAffine(0,
                 Div(COS_Q12(0) * 16, t != 0 ? t : 2),
                 Div(-SIN_Q12(0) * 16, t != 0 ? t : 2),
                 Div(SIN_Q12(0) * 16, t != 0 ? t : 2),
                 Div(COS_Q12(0) * 16, t != 0 ? t : 2));

    if (proc->unk4c <= 9)
        proc->unk4c++;
}
asm(".global sub_08040BB8\n.thumb_set sub_08040BB8, CaptureAnimPopup_Loop\n");
