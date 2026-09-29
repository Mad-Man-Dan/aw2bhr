#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080424BC.
 * sub_080424BC @ 0x080424BC, sub_080424E4 @ 0x080424E4
 */

void ResetCaptureProgressIfMoved(void)
{
    u32 cur = gUnknown_03003F24.raw;
    u32 want = gUnknown_03003100.raw;

    if (cur != want)
    {
        gUnknown_030040D8->unk05 &= 7;
    }
}
asm(".global sub_080424BC\n.thumb_set sub_080424BC, ResetCaptureProgressIfMoved\n");

void RecordUnitActionCell(void)
{
    gUnknown_03003F24.pos.unk00 = gUnknown_03003100.pos.unk00;
    gUnknown_03003F24.pos.unk02 = gUnknown_03003100.pos.unk02;
}
asm(".global sub_080424E4\n.thumb_set sub_080424E4, RecordUnitActionCell\n");
