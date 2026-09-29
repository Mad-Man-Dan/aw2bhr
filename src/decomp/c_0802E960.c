#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E960.
 * sub_0802E960 @ 0x0802E960
 */

#include "hardware.h"
#include "proc.h"

void OnMain_SioError(void)
{
    SetupBackgrounds(gUnknown_0849D16C);
    InitSoundSystem();
    Proc_Init();
    InitSlotScripts();

    gDispIo.disp_ct.bg0_enable = 1;
    gDispIo.disp_ct.bg1_enable = 0;
    gDispIo.disp_ct.bg2_enable = 0;
    gDispIo.disp_ct.bg3_enable = 0;
    gDispIo.disp_ct.obj_enable = 0;
    gDispIo.disp_ct.win0_enable = 0;
    gDispIo.disp_ct.win1_enable = 0;
    gDispIo.disp_ct.objwin_enable = 0;

    gUnknown_030030E0.bits.effect = 0;

    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    CpuFastSet(gUnknown_0816DB88, (void *)0x05000000, 8);
    Decompress(gUnknown_0849B020, (void *)0x06000000);
    Decompress(gUnknown_0849B024, (void *)0x06007000);

    gUnknown_03001FF8 = 0;
    gUnknown_03001418 = 0;

    FlushLCDControl();
    sub_080366C4(OnMain_SioErrorWait);
}
asm(".global sub_0802E960\n.thumb_set sub_0802E960, OnMain_SioError\n");
