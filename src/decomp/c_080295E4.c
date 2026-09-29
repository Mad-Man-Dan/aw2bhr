#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080295E4.
 * sub_080295E4 @ 0x080295E4
 */

void DropCellPicker_Init(void)
{
    gUnknown_03001470[gUnknown_03001FBC].unk1e =
        GetDropDirectionMask(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
                     gUnknown_030040D8->unk07[
                         gUnknown_03001470[gUnknown_03001FBC].unk22]);
    gUnknown_03001470[gUnknown_03001FBC].unk20 = 3;
    gUnknown_03001470[gUnknown_03001FBC].unk24 = 0;

    if (gPlaySt.savingEnabled != 0)
        SnapshotActionCommandContext();

    DropCellPicker_SelectNext();
}
asm(".global sub_080295E4\n.thumb_set sub_080295E4, DropCellPicker_Init\n");
