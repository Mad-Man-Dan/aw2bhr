#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C57C.
 * sub_0802C57C @ 0x0802C57C, sub_0802C594 @ 0x0802C594
 */

void BackupUnitStartPosition(void)
{
    gUnknown_030044A4.unk00 = gUnknown_03003F24.pos.unk00;
    gUnknown_030044A4.unk02 = gUnknown_03003F24.pos.unk02;
}
asm(".global sub_0802C57C\n.thumb_set sub_0802C57C, BackupUnitStartPosition\n");

void RestoreUnitStartPosition(void)
{
    gUnknown_03003F24.pos.unk00 = gUnknown_030044A4.unk00;
    gUnknown_03003F24.pos.unk02 = gUnknown_030044A4.unk02;
}
asm(".global sub_0802C594\n.thumb_set sub_0802C594, RestoreUnitStartPosition\n");
