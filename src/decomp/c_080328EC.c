#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080328EC.
 * sub_080328EC @ 0x080328EC
 */

void LinkMapPick_DrawPreview(void)
{
    if (DrawDesignRoomSlotPreviewBg1(0x200, gUnknown_0849B060->unk04) == 0)
    {
        FillTilemapRect(gBG1TilemapBuffer, 0, 0, 0x20, 0x14, 0);

        if (gUnknown_0849B060->unk09 != gUnknown_0849B018->unk06)
            sub_080328C0(gBG1TilemapBuffer + 0x83);

        BG_EnableSyncBG1();
    }
}
asm(".global sub_080328EC\n.thumb_set sub_080328EC, LinkMapPick_DrawPreview\n");
