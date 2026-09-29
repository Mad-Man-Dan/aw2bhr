#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074DAC.
 * sub_08074DAC @ 0x08074DAC
 */

#include "proc.h"
struct Unk8074DACProc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ s16 unk2a[5];
    /* 0x34 */ s16 unk34[5];
    /* 0x3e */ u8 filler_3e[2];
    /* 0x40 */ struct Unk0801C210 *unk40[5];
    /* 0x54 */ u8 filler_54[4];
    /* 0x58 */ int unk58;
};

void WorldMapScope_Loop(struct Unk8074DACProc *proc)
{
    int i;
    s16 x;
    s16 y;

    proc->unk2a[0] = gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00;
    proc->unk34[0] = gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02;

    x = gUnknown_0202FDFC.unk00;
    y = gUnknown_0202FDFC.unk02;

    if (proc->unk58 & 1)
    {
        for (i = 4; i > 0; i--)
        {
            if (proc->unk2a[i] != proc->unk2a[i - 1]
                || proc->unk34[i] != proc->unk34[i - 1])
            {
                AP_Update(proc->unk40[i], (proc->unk2a[i] - x) & 0x1FF,
                    (proc->unk34[i] - y) & 0xFF);

                proc->unk2a[i] = proc->unk2a[i - 1];
                proc->unk34[i] = proc->unk34[i - 1];
            }
        }
    }

    AP_Update(proc->unk40[0], (proc->unk2a[0] - x) & 0x1FF,
        (proc->unk34[0] - y) & 0xFF);

    proc->unk58++;
}
asm(".global sub_08074DAC\n.thumb_set sub_08074DAC, WorldMapScope_Loop\n");
