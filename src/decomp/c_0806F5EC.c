#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F5EC.
 * sub_0806F5EC @ 0x0806F5EC
 */

#include "hardware.h"
#include "proc.h"
struct Unk6F5ECProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};

void SoundRoomFadeToBlack_Init(struct Unk6F5ECProc * proc)
{
    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    *(vu16 *)&gUnknown_030030E0 = (*(vu16 *)&gUnknown_030030E0 & 0xFFE0) | 0x0F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk58 = 0;
}
asm(".global sub_0806F5EC\n.thumb_set sub_0806F5EC, SoundRoomFadeToBlack_Init\n");
