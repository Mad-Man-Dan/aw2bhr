#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806780C.
 * sub_0806780C @ 0x0806780C
 */

#include "proc.h"
struct Unk6780CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x5c);
    /* 5c */ u32 unk5c;
};

void ResumeIntroBgScroll(void)
{
    ((struct Unk6780CProc *)Proc_Find(gUnknown_08580FE4))->unk5c = 1;
}
asm(".global sub_0806780C\n.thumb_set sub_0806780C, ResumeIntroBgScroll\n");
