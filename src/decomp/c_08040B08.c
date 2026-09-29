#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040B08.
 * sub_08040B08 @ 0x08040B08
 */

#include "hardware.h"
#include "proc.h"
/* Installs the spinning sprite and seeds OAM affine slot 0 with the identity
 * rotation: the four Divs are the standard (cos, -sin, sin, cos) matrix at
 * angle 0, scaled by 16 over 0x100. Writing them as SIN_Q12/COS_Q12 is the
 * honest spelling -- COS_Q12(0) is exactly the `gSinLut + 0x80` the ROM loads
 * (0x80 bytes is 64 halfwords).
 *
 * Div is called FOUR times, not three plus a reuse: the first and last
 * arguments are the same expression, but Div is an ordinary call so nothing
 * CSEs across it. Each result is narrowed `lsls #0x10; asrs #0x10` into
 * SetObjAffine's s16 parameters, and the first three must survive the later
 * calls, which is what puts them in r6/r5/r4 and spills the fourth to the stack
 * as the fifth argument. */
struct Unk40B08Proc
{
    /* 00 */ u8 filler_00[0x30];
    /* 30 */ struct Unk0801C210 *unk30;
    /* 34 */ u8 filler_34[0x18];
    /* 4c */ u16 unk4c;
};

void CaptureAnimPopup_Init(struct Unk40B08Proc *proc)
{
    proc->unk30 = AP_Create((void *)gUnknown_08121870, 0, 1);
    AP_SwitchAnimation(proc->unk30, 1);
    proc->unk30->unk22 = 0x31CA;

    SetObjAffine(0,
                 Div(COS_Q12(0) * 16, 0x100),
                 Div(-SIN_Q12(0) * 16, 0x100),
                 Div(SIN_Q12(0) * 16, 0x100),
                 Div(COS_Q12(0) * 16, 0x100));

    proc->unk4c = 0;
}
asm(".global sub_08040B08\n.thumb_set sub_08040B08, CaptureAnimPopup_Init\n");
