#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076298.
 * sub_08076298 @ 0x08076298
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk8076298
{
    /* 0x00 */ u8 filler_00[0x3a];
    /* 0x3a */ s8 unk3a;
    /* 0x3b */ u8 filler_3b[0x5];
    /* 0x40 */ int unk40;
};

void WorldMapNationPanel_SlideOutLoop(ProcPtr procv)
{
    struct Unk8076298 *proc = procv;
    int n;
    u32 a;
    u32 b;

    a = 0;
    CpuFastSet(&a, gBG0TilemapBuffer + 0x200, 0x01000040);
    b = 0;
    CpuFastSet(&b, gBG2TilemapBuffer + 0x200, 0x01000040);

    BG_EnableSyncBG0();
    BG_EnableSyncBG2();

    n = gUnknown_0861445C[proc->unk40];

    if (proc->unk3a < 0)
    {
        sub_08071900(gUnknown_08551A00 + (0x14D - n), gBG0TilemapBuffer + 0x200,
                     n, 4);
        sub_08071900(gUnknown_08551A04 + (0x14D - n), gBG2TilemapBuffer + 0x200,
                     n, 4);
    }
    else
    {
        sub_08071900(gUnknown_08551A00 + 0x140, gBG0TilemapBuffer + (0x21E - n),
                     n, 4);
        sub_08071900(gUnknown_08551A04 + 0x140, gBG2TilemapBuffer + (0x21E - n),
                     n, 4);
    }

    proc->unk40++;

    if (proc->unk40 == 4)
    {
        proc->unk40 = 0;
        Proc_Break(proc);
    }
}

asm(".global sub_08076298\n.thumb_set sub_08076298, WorldMapNationPanel_SlideOutLoop\n");
