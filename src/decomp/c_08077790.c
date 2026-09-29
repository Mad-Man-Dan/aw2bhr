#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077790.
 * sub_08077790 @ 0x08077790, sub_08077818 @ 0x08077818, sub_08077870 @ 0x08077870, sub_08077954 @ 0x08077954
 */

#include "proc.h"
#include "hardware.h"
/* Wave 35 (W35-B). Restores the two frame buffers from the two saved blobs and
 * resets the step counter. `+ 0x80` is the ROM's `movs r4, #0x80; lsls r4, #1`
 * = 0x100 BYTES, which is index 0x80 on the `u16 *` the two buffer pointers
 * carry. The argument direction is the opposite of WorldMapMissionInfo_SlideOutForPreviewLoop's: here the
 * buffer is the destination. */
struct Unk77818Proc
{
    /* 00 */ u8 filler_00[0x44];
    /* 44 */ int unk44;
};
/* Wave 35 (W35-B). WorldMapMissionInfo_SlideOutForPreviewLoop with a scroll write and a teardown. The scroll
 * byte comes from gUnknown_086145E7, which is `u8 []` read through an explicit
 * (s8) cast -- the ROM's `ldrb` then `lsls #0x18; asrs #0x18`, where the
 * gUnknown_086145E2 read four instructions earlier in the same function uses
 * `ldrsb`. Two tables, two types, one function proving both. */
struct Unk77870Proc
{
    /* 00 */ u8 filler_00[0x44];
    /* 44 */ int unk44;
};
/* Wave 35 (W35-B). One frame of a five-step wipe: blank both buffers, then
 * blit `x` columns of each saved blob back at `0x9E - x`, where x comes from
 * the s8 table gUnknown_086145D8's sibling gUnknown_086145E2. The two zeroed
 * stack words are two separate `int` locals whose addresses are taken, not one
 * -- the ROM writes sp[0] and sp[4] from the same CSEd 0 and passes each to its
 * own CpuFastSet. WorldMapMissionInfo_SlideOutLoop is this function plus a scroll write and a
 * teardown. */
struct Unk77954Proc
{
    /* 00 */ u8 filler_00[0x44];
    /* 44 */ int unk44;
};

/* Wave 35 (W35-B). The "press anything" front end of the same screen
 * WorldMapMapPreview_WaitLoop drives: B breaks the proc, A and R jump to two different labels,
 * and sub_08014824 gates both of the latter. gpKeySt->pressed is re-loaded after
 * that call because the call clobbers it, and the two remaining bit tests share
 * the one reload. */
void WorldMapMissionInfo_InputLoop(ProcPtr proc)
{
    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);

    if (gpKeySt->pressed & 2)
    {
        sub_08014878();
        SetHeaderBannerTransitionFrames(5);
        PlayMusicOrSfx2(0x66);
        Proc_Break(proc);
    }
    else if (!sub_08014824())
    {
        if (gpKeySt->pressed & 1)
        {
            sub_08014878();
            PlayMusicOrSfx2(0x1CE);
            Proc_Goto(proc, 1);
        }
        else if (gpKeySt->pressed & R_BUTTON)
        {
            sub_08014878();
            Proc_Goto(proc, 2);
        }
    }
}
asm(".global sub_08077790\n.thumb_set sub_08077790, WorldMapMissionInfo_InputLoop\n");

void WorldMapMissionInfo_CopyPanelToBuffer(struct Unk77818Proc *proc)
{
    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);
    sub_08071900(gBG0TilemapBuffer + 0x80, gUnknown_08551A00, 0x1E, 7);
    sub_08071900(gBG2TilemapBuffer + 0x80, gUnknown_08551A04, 0x1E, 7);
    proc->unk44 = 0;
}
asm(".global sub_08077818\n.thumb_set sub_08077818, WorldMapMissionInfo_CopyPanelToBuffer\n");

void WorldMapMissionInfo_SlideOutLoop(struct Unk77870Proc *proc)
{
    int a;
    int b;
    int x;

    a = 0;
    CpuFastSet(&a, gBG0TilemapBuffer + 0x80, 0x01000070);
    b = 0;
    CpuFastSet(&b, gBG2TilemapBuffer + 0x80, 0x01000070);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();

    x = gUnknown_086145E2[proc->unk44];
    sub_08071900(gUnknown_08551A00, gBG0TilemapBuffer + (0x9E - x), x, 7);
    sub_08071900(gUnknown_08551A04, gBG2TilemapBuffer + (0x9E - x), x, 7);

    gUnknown_0300064C = (s8)gUnknown_086145E7[proc->unk44];
    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);

    if (++proc->unk44 == 5)
    {
        SetVCountInterruptHandler(0);
        proc->unk44 = 0;
        SetDifficultyStarsPalette(0);
        SetWorldMapScopePalette(0);
        Proc_Break(proc);
    }
}
asm(".global sub_08077870\n.thumb_set sub_08077870, WorldMapMissionInfo_SlideOutLoop\n");

void WorldMapMissionInfo_SlideOutForPreviewLoop(struct Unk77954Proc *proc)
{
    int a;
    int b;
    int x;

    a = 0;
    CpuFastSet(&a, gBG0TilemapBuffer + 0x80, 0x01000070);
    b = 0;
    CpuFastSet(&b, gBG2TilemapBuffer + 0x80, 0x01000070);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();

    x = gUnknown_086145E2[proc->unk44];
    sub_08071900(gUnknown_08551A00, gBG0TilemapBuffer + (0x9E - x), x, 7);
    sub_08071900(gUnknown_08551A04, gBG2TilemapBuffer + (0x9E - x), x, 7);

    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);

    if (++proc->unk44 == 5)
    {
        proc->unk44 = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08077954\n.thumb_set sub_08077954, WorldMapMissionInfo_SlideOutForPreviewLoop\n");
