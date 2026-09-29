#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C474.
 * sub_0806C474 @ 0x0806C474
 */

#include "hardware.h"
#include "proc.h"

void sub_0806C474(void)
{
    Proc_EndEach(gUnknown_085819D4);

    gDispIo.disp_ct.bg0_enable = 0;
    gDispIo.disp_ct.bg1_enable = 0;
    gDispIo.disp_ct.bg2_enable = 0;
    gDispIo.disp_ct.bg3_enable = 1;
    gDispIo.disp_ct.obj_enable = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    sub_0801237C();
    sub_08012358();

    ApplyPaletteExt(gUnknown_081A29E4, 0, 0x20);
    Decompress(gUnknown_081A23B4, (void *)0x06008000);
    Decompress(gUnknown_081A2854, (void *)0x0600F800);

    SetBgScrollShadow(3, 0, 0);
}
