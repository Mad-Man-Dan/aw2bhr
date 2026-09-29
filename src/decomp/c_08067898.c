#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067898.
 * sub_08067898 @ 0x08067898
 */

#include "proc.h"
/* Same script as SetIntroBgZoomEnabled (+0x60) and SetIntroBgZoomAcceleration (+0x3c), which is what
 * fixes +0x3c and +0x60 as words rather than something narrower.
 */
struct Unk67898Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x3c);
    /* 3c */ u32 unk3c;
    /* 40 */ STRUCT_PAD(0x40, 0x58);
    /* 58 */ u32 unk58;
    /* 5c */ u32 unk5c;
    /* 60 */ u32 unk60;
};

/* Three payload words in r0-r2 and the parent in r3, so r1 is set up from the
 * fourth parameter rather than left alone.
 */
void StartIntroBgZoom(u32 a, u32 b, u32 c, ProcPtr parent)
{
    struct Unk67898Proc *proc = Proc_Start(gUnknown_08580FF4, parent);

    proc->unk58 = a;
    proc->unk5c = b;
    proc->unk60 = 1;
    proc->unk3c = c;
}
asm(".global sub_08067898\n.thumb_set sub_08067898, StartIntroBgZoom\n");
