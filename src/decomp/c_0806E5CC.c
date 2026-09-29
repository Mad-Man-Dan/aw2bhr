#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E5CC.
 * sub_0806E5CC @ 0x0806E5CC
 */

#include "proc.h"
/* The `lsl #16; lsr #16` on the argument before the `bl` is PROMOTE_MODE on a
 * u16 parameter that has to survive a call. r1 is never written before the
 * `bl`, so the parent is this starter's own second parameter. */
struct Unk6E5CCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ u16 unk34;
};

void StartSoundRoomCoPortrait(u16 a, ProcPtr parent)
{
    struct Unk6E5CCProc * proc = Proc_Start(gUnknown_08582B2C, parent);

    proc->unk34 = a;
    proc->unk2c = 0x140;
    proc->unk30 = 0xa0;
}
asm(".global sub_0806E5CC\n.thumb_set sub_0806E5CC, StartSoundRoomCoPortrait\n");
