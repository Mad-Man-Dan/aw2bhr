#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004E88.
 * sub_08004E88 @ 0x08004E88
 */

void DesignRoomLoadFromSlot(void)
{
    LoadDesignRoomSlot(gActiveMap->designSlot, gDesignRoomName);
    sub_0800CB30(1, GetArmyColorSetIndex());
    sub_08004E38((char *)gActiveMap->designName, (const char *)gDesignRoomName);
    RecountArmyProperties();
    RegisterArmyHqs();
    gActiveMap->propertyCount = CountProperties();
    gActiveMap->flags &= 0xEFFF;
}
asm(".global sub_08004E88\n.thumb_set sub_08004E88, DesignRoomLoadFromSlot\n");
