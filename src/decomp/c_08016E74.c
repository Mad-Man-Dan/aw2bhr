#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016E74.
 * sub_08016E74 @ 0x08016E74
 */

void BackupBattleMapPoints(void)
{
    gUnknown_0200C500[0] = gUnknown_0200C420.unk00;
    gUnknown_0200C500[1] = gUnknown_0200C420.unk04;
}
asm(".global sub_08016E74\n.thumb_set sub_08016E74, BackupBattleMapPoints\n");
