#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015C30.
 * sub_08015C30 @ 0x08015C30
 */

void ClearSlotScriptCallback(u8 a1)
{
    gUnknown_03001470[a1].unk08 = 0;
}
asm(".global sub_08015C30\n.thumb_set sub_08015C30, ClearSlotScriptCallback\n");
