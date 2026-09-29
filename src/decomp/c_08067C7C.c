#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067C7C.
 * sub_08067C7C @ 0x08067C7C
 */

#include "proc.h"
struct Unk67C7CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x38);
    /* 38 */ u32 unk38;
};

void SetIntroSlidePanelExitFrames(u32 a)
{
    ((struct Unk67C7CProc *)Proc_Find(gUnknown_08581068))->unk38 = a;
}
asm(".global sub_08067C7C\n.thumb_set sub_08067C7C, SetIntroSlidePanelExitFrames\n");
