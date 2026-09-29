#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035F68.
 * sub_08035F68 @ 0x08035F68
 */

#include "proc.h"
/* Both arms call PlayMoveSlideMoveSfx and the ROM has it TWICE, so the tail really was
 * written twice in the source -- cross-jumping would have merged them had they
 * shared a block. The arms REJOIN rather than return, so the natural sense is
 * the matching one (the wave-26 qualifier to the arm-placement rule). */
struct Unk35F68Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x36);
    /* 0x36 */ u8 unk36;
};

void StartMoveSlideMoveSound(ProcPtr procArg)
{
    struct Unk35F68Proc *proc = procArg;

    if (gUnknown_0849CD88[proc->unk36].unk1e != 0x8000)
    {
        sub_0801537C(gUnknown_0849BDE8);
        PlayMoveSlideMoveSfx(proc);
    }
    else
        PlayMoveSlideMoveSfx(proc);
}
asm(".global sub_08035F68\n.thumb_set sub_08035F68, StartMoveSlideMoveSound\n");
