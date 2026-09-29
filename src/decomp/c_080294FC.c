#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080294FC.
 * sub_080294FC @ 0x080294FC, sub_08029570 @ 0x08029570
 */

void DropCellPicker_SelectNext(void)
{
    u8 old;
    int v;

    old = gUnknown_03001470[gUnknown_03001FBC].unk20;

    do
    {
        gUnknown_03001470[gUnknown_03001FBC].unk20 =
            (u8)(gUnknown_03001470[gUnknown_03001FBC].unk20 + 1) & 3;
    } while ((gUnknown_03001470[gUnknown_03001FBC].unk1e
        & (s8)gUnknown_0849A06C[
            (v = gUnknown_03001470[gUnknown_03001FBC].unk20) * 5]) == 0);

    if (old != v)
        PlayMusicOrSfx2(0x6a);
}
asm(".global sub_080294FC\n.thumb_set sub_080294FC, DropCellPicker_SelectNext\n");

void DropCellPicker_SelectPrevious(void)
{
    u8 old;
    int v;

    old = gUnknown_03001470[gUnknown_03001FBC].unk20;

    do
    {
        gUnknown_03001470[gUnknown_03001FBC].unk20 =
            (u8)(gUnknown_03001470[gUnknown_03001FBC].unk20 - 1) & 3;
    } while ((gUnknown_03001470[gUnknown_03001FBC].unk1e
        & (s8)gUnknown_0849A06C[
            (v = gUnknown_03001470[gUnknown_03001FBC].unk20) * 5]) == 0);

    if (old != v)
        PlayMusicOrSfx2(0x6a);
}
asm(".global sub_08029570\n.thumb_set sub_08029570, DropCellPicker_SelectPrevious\n");
