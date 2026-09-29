#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08036024.
 * sub_08036024 @ 0x08036024
 */

#include "proc.h"
struct Unk36024Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x36);
    /* 0x36 */ u8 unk36;
};

void PlayMoveSlideStopSfx(ProcPtr procArg)
{
    struct Unk36024Proc *proc = procArg;
    struct Unk03001470 *p;

    switch (gUnknown_0849CD88[proc->unk36].unk1e)
    {
    case 0:
        break;

    case 0x8000:
        PlayMoveSlideMoveSfx(proc);
        break;

    case 0x4000:
        p = sub_080152EC(gUnknown_0849BDE8, 0);
        p->unk18 = (int)gUnknown_0849CD88[proc->unk36].unk20;
        break;

    default:
        PlayMusicOrSfx(gUnknown_0849CD88[proc->unk36].unk1e);
        break;
    }
}
asm(".global sub_08036024\n.thumb_set sub_08036024, PlayMoveSlideStopSfx\n");
