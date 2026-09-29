#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066F20.
 * sub_08066F20 @ 0x08066F20
 */

void MatchSetupShowHelpText(void)
{
    u16 id;

    if (gUnknown_08580934->unk30 == 0)
    {
        if (gUnknown_08580934->unk33 != 6)
        {
            id = gUnknown_08580D6C[gUnknown_08580934->unk33];
        }
        else
        {
            struct Unk08580934_Obj *o;

            o = gUnknown_08580934->unk54[6];
            id = gUnknown_08580D88[o->unk48];
        }
    }
    else if (gUnknown_08580934->unk26 == 0)
    {
        id = gUnknown_08580D78[gUnknown_08580934->unk32];
    }
    else
    {
        id = 0x9db;
    }

    if (id != gUnknown_08580934->unk2e)
    {
        sub_08014878();
        FillTilemapRect(gBG0TilemapBuffer, 3, 0x11, 0x1a, 3, 0);
        StartTextBox(3, 0x11, gBG0TilemapBuffer, id, 0x3000, 0x100)->unk3a = 2;
        gUnknown_08580934->unk2e = id;
    }
}
asm(".global sub_08066F20\n.thumb_set sub_08066F20, MatchSetupShowHelpText\n");
