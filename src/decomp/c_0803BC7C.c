#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BC7C.
 * sub_0803BC7C @ 0x0803BC7C, sub_0803BC88 @ 0x0803BC88, sub_0803BC94 @ 0x0803BC94
 */

u8 GetCampaignSaveFlag(void)
{
    return gUnknown_03003F30[1];
}
asm(".global sub_0803BC7C\n.thumb_set sub_0803BC7C, GetCampaignSaveFlag\n");

u8 GetVersusSaveFlag(void)
{
    return gUnknown_03003F30[3];
}
asm(".global sub_0803BC88\n.thumb_set sub_0803BC88, GetVersusSaveFlag\n");

u8 GetWarRoomSaveFlag(void)
{
    return gUnknown_03003F30[2];
}
asm(".global sub_0803BC94\n.thumb_set sub_0803BC94, GetWarRoomSaveFlag\n");
