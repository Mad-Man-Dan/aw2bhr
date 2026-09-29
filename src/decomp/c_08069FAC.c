#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069FAC.
 * sub_08069FAC @ 0x08069FAC
 */

#include "proc.h"
/* r1 is never written before the `bl`, so the parent is this starter's own
 * fourth parameter, passed straight through.
 * The parameters are saved into r6/r4/r5 rather than r4/r5/r6: the second
 * argument is stored first, and the allocation follows from that store order,
 * not from the parameter order. */
struct Unk69FACProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ s32 unk34;
    /* 38 */ s32 unk38;
    /* 3c */ s32 unk3c;
    /* 40 */ s32 unk40;
};

void StartIntroCoReveal(s32 a, s32 b, s32 c, ProcPtr parent)
{
    struct Unk69FACProc * proc = Proc_Start(gUnknown_08581480, parent);

    proc->unk34 = b;
    proc->unk38 = a;
    proc->unk3c = c;
    proc->unk40 = 0;
}
asm(".global sub_08069FAC\n.thumb_set sub_08069FAC, StartIntroCoReveal\n");
