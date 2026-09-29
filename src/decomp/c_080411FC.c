#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080411FC.
 * sub_080411FC @ 0x080411FC
 */

#include "proc.h"
/* Either restarts the proc's script from the top, or installs the unit art and
 * starts the two follow-on procs, handing the cursor pair at +0x3c/+0x3e to the
 * gUnknown_0849FD14 one (the script whose PROC_ONEND callback is the matched
 * CaptureAnimPopup_OnEnd).
 *
 * All four `adds rN, #imm` are THUMB displacement limits -- ldrb reaches only
 * 0..31 -- and not addresses being taken; +0x4a is bound to r5 because it is
 * read twice, once for PlayCaptureCompleteSound and again for LoadCapturePropertySprite. */
struct Unk411FCProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x3c);
    /* 0x3c */ u16 unk3c;
    /* 0x3e */ u16 unk3e;
    /* 0x40 */ u8 filler_40[0x04];
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 filler_45[0x04];
    /* 0x49 */ u8 unk49;
    /* 0x4a */ u8 unk4a;
};

void CaptureAnim_CheckComplete(struct Unk411FCProc *proc)
{
    struct Unk411FCProc *newProc;

    if (proc->unk49 <= 0x13)
    {
        Proc_Goto(proc, 0);
    }
    else
    {
        PlayCaptureCompleteSound(proc->unk44, proc->unk4a);
        LoadCapturePropertySprite(proc->unk4a, 0x22A, 5);

        Proc_StartBlocking(gUnknown_0849FE54, proc);

        newProc = Proc_Start(gUnknown_0849FD14, proc);
        newProc->unk3c = proc->unk3c;
        newProc->unk3e = proc->unk3e;
    }
}
asm(".global sub_080411FC\n.thumb_set sub_080411FC, CaptureAnim_CheckComplete\n");
