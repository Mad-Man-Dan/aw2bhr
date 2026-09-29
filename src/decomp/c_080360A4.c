#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080360A4.
 * sub_080360A4 @ 0x080360A4
 */

#include "proc.h"
/* StartMoveSlideMoveSound's twin, with only the `== 0x8000` arm: that one calls
 * PlayMoveSlideMoveSfx on both sides of the test and so has it twice, this one calls it
 * once under the equality. */
struct Unk360A4Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x36);
    /* 0x36 */ u8 unk36;
};

void PlayMoveSlideStepSfx(ProcPtr procArg)
{
    struct Unk360A4Proc *proc = procArg;

    if (gUnknown_0849CD88[proc->unk36].unk1e == 0x8000)
        PlayMoveSlideMoveSfx(proc);
}
asm(".global sub_080360A4\n.thumb_set sub_080360A4, PlayMoveSlideStepSfx\n");
