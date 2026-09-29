#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804138C.
 * sub_0804138C @ 0x0804138C, sub_08041398 @ 0x08041398
 */

void ClearAttackTargetList(void)
{
    gUnknown_030040A8 = 0;
}
asm(".global sub_0804138C\n.thumb_set sub_0804138C, ClearAttackTargetList\n");

u32 GetAttackTargetCount(void)
{
    return gUnknown_030040A8;
}
asm(".global sub_08041398\n.thumb_set sub_08041398, GetAttackTargetCount\n");
