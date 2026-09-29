#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D918.
 * sub_0802D918 @ 0x0802D918
 */

void sub_0802D918(void)
{
    struct Unk03001470 *proc;

    sub_08024268();
    SaveMapCursorPosition();

    gUnknown_03001418 = gUnknown_03001FF8 = 0;

    proc = &gUnknown_03001470[gUnknown_03001FBC];
    proc->unk1e = 0;
    proc->unk20 = 0;
    proc->unk22 = gUnknown_0300055A - 1;

    sub_0802D7B0();
    sub_0801A444(1, 4, 0xf, 0x10);
    sub_0802D7B4(0);
    sub_08022AD0(8, 0x28);
    sub_0803B4DC(0x65);
    sub_0803A9C8(gUnknown_02023830[0]);
    sub_0802776C(1);
}
