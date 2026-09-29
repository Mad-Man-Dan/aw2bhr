#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033470.
 * sub_08033470 @ 0x08033470
 */

#include "proc.h"
#include "hardware.h"
struct Unk33470Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

void LinkMultiboot_SendImageThenBoot(struct Unk33470Proc *proc)
{
    vu32 *dma;
    u32 fill;
    int zero;
    u32 magic;

    zero = 0;

    if (proc->unk64 == 1)
    {
        MultiBootStartMaster((struct Unk08062FB8 *)gUnknown_03003F70,
            (int)(gUnknown_030032DC + 0xc0), gUnknown_03003F28 - 0xc0, 4, 1);
        proc->unk64 = 2;
    }

    MultiBootMain(gUnknown_03003F70);

    if (sub_08063518((struct Unk08062FB8 *)gUnknown_03003F70))
    {
        REG_IME = zero;
        REG_IF = 0xFFFF;

        fill = zero;
        dma = (vu32 *)(REG_BASE + REG_OFFSET_DMA3SAD);

        dma[0] = (u32)&fill;
        dma[1] = 0x02000000;
        dma[2] = 0x85010000;
        dma[2];

        dma[0] = (u32)gUnknown_08090D5C[2];
        dma[1] = 0x02000000;
        dma[2] = 0x84000000 | ((gUnknown_08090D5C[3] - gUnknown_08090D5C[2]) / 4);
        dma[2];

        Decompress(gUnknown_03003F44, gUnknown_0203C000);
        magic = 0x485153CD;
        gUnknown_0203BFFC = magic;
        ((void (*)(void))gUnknown_0203C000)();

        Proc_Break(proc);
    }
}
asm(".global sub_08033470\n.thumb_set sub_08033470, LinkMultiboot_SendImageThenBoot\n");
