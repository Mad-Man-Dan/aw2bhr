#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080604BC.
 * sub_080604BC @ 0x080604BC
 */

void sub_080604BC(void)
{
    StartUnrecordedUnitAttack(gUnknown_030046C0.unk06);
    ClearSlotScriptCallback(gUnknown_03001FBC);
}
