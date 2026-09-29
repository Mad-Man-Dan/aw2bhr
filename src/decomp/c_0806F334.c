#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F334.
 * sub_0806F334 @ 0x0806F334
 */

#include "hardware.h"
struct Unk806F334
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ u8 unk38;
};

void SoundRoomGallery_Init(struct Unk806F334 *proc)
{
    gDispIo.disp_ct.bg0_enable = 1;
    gDispIo.disp_ct.bg1_enable = 0;
    gDispIo.disp_ct.bg2_enable = 0;
    gDispIo.disp_ct.bg3_enable = 0;
    gDispIo.disp_ct.obj_enable = 1;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    ApplyBgControlTable(gUnknown_085819E4);
    SetBgScrollShadow(0, 0, 0);

    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 1;
    gUnknown_030030A4.bits.win1_enable_bg2 = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 1;
    gUnknown_030030A4.bits.win1_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    StartSoundRoomArrows(1, proc);

    proc->unk38 = gUnknown_0202F2D8;
}
asm(".global sub_0806F334\n.thumb_set sub_0806F334, SoundRoomGallery_Init\n");
