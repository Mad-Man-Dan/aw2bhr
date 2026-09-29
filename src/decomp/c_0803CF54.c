#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CF54.
 * sub_0803CF54 @ 0x0803CF54
 */

void SaveDesignRoomSlot(u8 a1, const void *a2, u8 a3)
{
    sub_0803CFA4(a2, gUnknown_02000000, a3);
    if (gUnknown_030040A0 == 0)
        gUnknown_02000000[0x4C3] = GetLoadedMapArmyCount();
    sub_0801A7D8(a1 + 5, gUnknown_02000000, 0x724);
    sub_0803D48C();
}
asm(".global sub_0803CF54\n.thumb_set sub_0803CF54, SaveDesignRoomSlot\n");
