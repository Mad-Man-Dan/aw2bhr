#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077690.
 * sub_08077690 @ 0x08077690
 */

#include "proc.h"
/* Wave 35 (W35-B). The WorldMapMissionInfo_SlideOutLoop shape with sub_08071918 doing the blanking
 * instead of a pair of CpuFastSets, so there is no stack frame at all, and with
 * the 086145D8/086145DD table pair in place of 086145E2/086145E7. */
struct Unk77690Proc
{
    /* 00 */ u8 filler_00[0x44];
    /* 44 */ int unk44;
};

void WorldMapMissionInfo_SlideInLoop(struct Unk77690Proc *proc)
{
    int x;

    sub_08071918(gBG0TilemapBuffer + 0x80, 0x1E, 7, 0);
    sub_08071918(gBG2TilemapBuffer + 0x80, 0x1E, 7, 0);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();

    x = gUnknown_086145D8[proc->unk44];
    sub_08071900(gUnknown_08551A00, gBG0TilemapBuffer + (0x9E - x), x, 7);
    sub_08071900(gUnknown_08551A04, gBG2TilemapBuffer + (0x9E - x), x, 7);

    gUnknown_0300064C = (s8)gUnknown_086145DD[proc->unk44];
    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);

    if (++proc->unk44 == 5)
    {
        proc->unk44 = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08077690\n.thumb_set sub_08077690, WorldMapMissionInfo_SlideInLoop\n");
