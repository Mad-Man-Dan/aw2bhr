#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807249C.
 * sub_0807249C @ 0x0807249C
 */

#include "proc.h"
struct Unk8613F0CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ u32 unk58;
};

void StartTemporaryLock(ProcPtr parent, u32 a)
{
    ((struct Unk8613F0CProc *)Proc_StartBlocking(gUnknown_08613F0C, parent))->unk58 = a;
}
asm(".global sub_0807249C\n.thumb_set sub_0807249C, StartTemporaryLock\n");
