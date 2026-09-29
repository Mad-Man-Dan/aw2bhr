#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C2D8.
 * sub_0802C2D8 @ 0x0802C2D8
 */

#include "hardware.h"
struct Unk2C2D8Proc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
};

void MinimapScreen_Init(struct Unk2C2D8Proc *proc)
{
    RebuildMapUnitLayers2();

    proc->unk20 = gMap->scrollX;
    proc->unk22 = gMap->scrollY;

    SnapshotTeamColorsFromPlayers();
    ShowMapPreview(0x06000000 + gUnknown_03002B6C.bits.chr_block * 0x4000,
        (int)gBG0TilemapBuffer, 1, 9);
    sub_08013AD4(0);
    ResetWindowShadows();
    SetDefaultColorEffects();

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;

    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target1_enable_bg3 = 1;
    gUnknown_030030E0.bits.target1_enable_obj = 1;

    gUnknown_03001FFC = 0;
}
asm(".global sub_0802C2D8\n.thumb_set sub_0802C2D8, MinimapScreen_Init\n");
