#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806DF58.
 * sub_0806DF58 @ 0x0806DF58
 */

void RulesScreenShowHelpText(void)
{
    u16 id;

    if (gUnknown_08580934->unk30 == 0)
    {
        if (gUnknown_08580934->unk33 != 6)
            id = gUnknown_08581F68[gUnknown_08580934->unk33];
        else
            id = gUnknown_08581F74[gUnknown_08580934->unk02];
    }
    else
    {
        id = 0x9de;
    }

    if (id != gUnknown_08580934->unk2e)
    {
        sub_08014878();
        FillTilemapRect(gBG0TilemapBuffer, 3, 0x11, 0x1a, 2, 0);
        StartTextBox(3, 0x11, gBG0TilemapBuffer, id, 0x3000, 0x100)->unk3a = 2;
        gUnknown_08580934->unk2e = id;
    }
}
asm(".global sub_0806DF58\n.thumb_set sub_0806DF58, RulesScreenShowHelpText\n");
