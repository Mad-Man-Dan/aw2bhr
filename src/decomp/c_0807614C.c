#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807614C.
 * sub_0807614C @ 0x0807614C, sub_080761C8 @ 0x080761C8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* Re-samples the camera-relative cursor cell each frame and, when it has moved,
 * either redraws (WorldMapNationPanel_Refresh + WorldMapNationPanel_Draw) or breaks the proc.
 *
 * The two sums are spelled with the FAR member first (`unk04 + unk00`), exactly
 * as c_0807610C.c spells its pair and for the reason that file records: agbcc
 * evaluates the SECOND operand first, so naming +0x00 second is what puts its
 * `ldrsh` after +0x04's. `ldrsh` rather than c_0807610C.c's `ldrh` because the
 * sum is then divided by 8, and the `cmp #0; bge; adds #7` biasing is
 * rounding-toward-zero signed division -- `>> 3` would be a bare `asrs`.
 *
 * The saved pair is written with CHAINED assignments (`proc->unk32 = ox =
 * proc->unk36`). Split into two statements agbcc gives the old value its own
 * pseudo and copies it (`ldrh r0; adds r5, r0, #0`), which is 4 bytes; chained,
 * the one `ldrh` lands directly in the register the later compare reads. The
 * load is `ldrh` on an s16 field because its only consumers are a halfword
 * store and a compare, and the sign extension reappears as `lsls #0x10; asrs
 * #0x10` at that compare -- the same reading c_08075C98.c gives its +0x4c. */
struct Unk807614C
{
    /* 0x00 */ u8 filler_00[0x32];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3a */ s8 unk3a;
};
/* One sub_08071900 pair per axis of a sign test on +0x3a, then a four-step
 * counter at +0x40 that breaks the proc when it wraps.
 * The two arms differ only in WHICH side carries the moving index: below zero
 * it is the gUnknown_08551A00/04 pair that is indexed by `0x14D - n` and the
 * gBG0TilemapBuffer/80 pair that takes a fixed +0x200, and at or above zero the
 * roles swap to a fixed +0x140 and `0x21E - n`. All four globals are `u16 *`,
 * so the ROM's byte offsets 0x400 and 0x280 are element counts 0x200 and 0x140.
 * `n` is an int: it is read `ldrsb` and handed to sub_08071900's third
 * parameter with no re-narrowing. */
struct Unk80761C8
{
    /* 0x00 */ u8 filler_00[0x3a];
    /* 0x3a */ s8 unk3a;
    /* 0x3b */ u8 filler_3b[0x5];
    /* 0x40 */ int unk40;
};

void WorldMapNationPanel_WatchLoop(struct Unk807614C *proc)
{
    s16 ox;
    s16 oy;
    int v;

    proc->unk32 = ox = proc->unk36;
    proc->unk34 = oy = proc->unk38;

    proc->unk36 = (gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00) / 8;
    proc->unk38 = (gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02) / 8;

    if (proc->unk36 == ox && proc->unk38 == oy)
        return;

    v = GetWorldMapNationPanelSide();

    if (proc->unk3a == v || v == 0)
    {
        WorldMapNationPanel_Refresh(proc);
        WorldMapNationPanel_Draw((struct Unk807606C *)proc);
    }
    else
    {
        Proc_Break(proc);
    }
}

void WorldMapNationPanel_SlideInLoop(struct Unk80761C8 *proc)
{
    int n = gUnknown_08614458[proc->unk40];

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

    BG_EnableSyncBG0();
    BG_EnableSyncBG2();

    proc->unk40++;

    if (proc->unk40 == 4)
    {
        proc->unk40 = 0;
        Proc_Break(proc);
    }
}

asm(".global sub_080761C8\n.thumb_set sub_080761C8, WorldMapNationPanel_SlideInLoop\n"
    ".global sub_0807614C\n.thumb_set sub_0807614C, WorldMapNationPanel_WatchLoop\n");
