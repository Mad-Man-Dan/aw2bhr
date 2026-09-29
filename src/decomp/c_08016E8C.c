#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016E8C.
 * sub_08016E8C @ 0x08016E8C
 */

void RestoreBattleMapPoints(void)
{
    gUnknown_0200C420.unk00 = gUnknown_0200C500[0];
    gUnknown_0200C420.unk04 = gUnknown_0200C500[1];
}
asm(".global sub_08016E8C\n.thumb_set sub_08016E8C, RestoreBattleMapPoints\n");
