#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074D28.
 * sub_08074D28 @ 0x08074D28
 */

#include "proc.h"
struct Unk8074D28Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ s16 unk2a[5];
    /* 0x34 */ s16 unk34[5];
    /* 0x3e */ u8 filler_3e[2];
    /* 0x40 */ struct Unk0801C210 *unk40[5];
    /* 0x54 */ int unk54;
};

void WorldMapScope_Init(struct Unk8074D28Proc *proc)
{
    struct Unk0801C210 *sprite;
    int i;

    for (i = 0; i <= 4; i++)
    {
        sprite = AP_Create(gUnknown_081D2930, 0, 1);
        sprite->unk22 = gUnknown_0861433C[proc->unk54];
        AP_SwitchAnimation(sprite, 0);
        sprite->unk0c += i;
        proc->unk40[i] = sprite;
        proc->unk2a[i] = gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00;
        proc->unk34[i] = gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02;
    }
}
asm(".global sub_08074D28\n.thumb_set sub_08074D28, WorldMapScope_Init\n");
