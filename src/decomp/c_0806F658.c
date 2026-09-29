#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F658.
 * sub_0806F658 @ 0x0806F658
 */

#include "hardware.h"
#include "proc.h"
struct Unk6F658Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};

void SoundRoomFadeFromBlack_Init(struct Unk6F658Proc * proc)
{
    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x10;

    *(vu16 *)&gUnknown_030030E0 = (*(vu16 *)&gUnknown_030030E0 & 0xFFE0) | 0x0F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk58 = 0x10;
}
asm(".global sub_0806F658\n.thumb_set sub_0806F658, SoundRoomFadeFromBlack_Init\n");
