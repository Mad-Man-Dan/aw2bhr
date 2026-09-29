#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026584.
 * sub_08026584 @ 0x08026584
 */

#include "unknown-functions.h"

void AddArmyBonusPointsNoOp(u8 a0, u16 a1)
{
}
asm(".global sub_08026584\n.thumb_set sub_08026584, AddArmyBonusPointsNoOp\n");
