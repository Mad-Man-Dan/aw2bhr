#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B55C.
 * sub_0803B55C @ 0x0803B55C
 */

#include "proc.h"
/* Only the halfword at +0x64 is ever touched here; the rest of the proc is
 * whatever struct Proc already describes. */
struct UnkB55CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

/* The argument is int, not s16: it survives the bl in a bare `adds r4, r0, #0`
 * with no PROMOTE_MODE narrowing, and an s16 parameter adds `lsl #16; lsr #16`
 * ahead of the call. The caller EventOp_PlayMusic feeds it an `ldrsh` result. */
void PlayMusicAfterFade(int a)
{
    struct UnkB55CProc *proc = Proc_Start(gUnknown_0849E728, PROC_TREE_3);

    proc->unk64 = a;
}
asm(".global sub_0803B55C\n.thumb_set sub_0803B55C, PlayMusicAfterFade\n");
