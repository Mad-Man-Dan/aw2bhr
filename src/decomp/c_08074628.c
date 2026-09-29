#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074628.
 * sub_08074628 @ 0x08074628
 */

#include "hardware.h"
struct Unk8074628
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk_4c;
};

void DarkenRampUp_Init(struct Unk8074628 * proc)
{
    proc->unk_4c = 0;

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03001FFC = 0;
}
asm(".global sub_08074628\n.thumb_set sub_08074628, DarkenRampUp_Init\n");
