#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017E74.
 * sub_08017E74 @ 0x08017E74, sub_08017E80 @ 0x08017E80
 */

void EnableScriptedInput(void)
{
    gUnknown_03001404 = 1;
}
asm(".global sub_08017E74\n.thumb_set sub_08017E74, EnableScriptedInput\n");

void DisableScriptedInput(void)
{
    gUnknown_03001404 = 0;
}
asm(".global sub_08017E80\n.thumb_set sub_08017E80, DisableScriptedInput\n");
