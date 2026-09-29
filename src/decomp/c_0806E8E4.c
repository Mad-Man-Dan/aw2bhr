#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E8E4.
 * sub_0806E8E4 @ 0x0806E8E4
 */

#include "proc.h"
/* Kicks one of two timers on the proc StartSoundRoomArrows starts -- the same script,
 * so the layout is that starter's, which zeroes unk58 and unk5c and is the
 * producer for both fields read here.
 *
 * Both arms JOIN at the epilogue, so the out-of-line block past the pool is the
 * ELSE: `beq` reaches the unk58 store, making unk5c the `if` body and the
 * condition `!= 0`. The two arms are otherwise identical, so this is exactly
 * the size-neutral mistake that only the branch sense catches. */
struct Unk6E8E4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
    /* 5c */ s32 unk5c;
};

void FlickSoundRoomArrow(int a)
{
    struct Unk6E8E4Proc *proc = Proc_Find(gUnknown_08582C24);

    if (a != 0)
        proc->unk5c = 0x10;
    else
        proc->unk58 = 0x10;
}
asm(".global sub_0806E8E4\n.thumb_set sub_0806E8E4, FlickSoundRoomArrow\n");
