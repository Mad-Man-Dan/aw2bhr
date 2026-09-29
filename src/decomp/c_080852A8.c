#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080852A8.
 * sub_080852A8 @ 0x080852A8
 */

struct Unk080852A8
{
    /* 0x00 */ u8 filler_00[0x66];
    /* 0x66 */ s16 unk66;
};

void CoInfoScreen_DrawPageText(struct Unk080852A8 *proc)
{
    if (gUnknown_03005940 <= 3)
        StartTextBoxViaRecord(1, 7, gBG0TilemapBuffer,
                     gUnknown_08616F0C[gPlayers[proc->unk66].co * 4
                                       + gUnknown_03005940],
                     0x8000, 0x3F);
    if (gUnknown_03005940 == 2)
    {
        InitTextTileCache(0);
        PutTextScriptImmediate(3, 5, gBG0TilemapBuffer,
                     gTextTable[gUnknown_085D3DD0[gPlayers[proc->unk66].co]
                                           .power[1].powerNameId],
                     0x8000, 0);
    }
    if (gUnknown_03005940 == 3)
    {
        InitTextTileCache(0);
        PutTextScriptImmediate(3, 5, gBG0TilemapBuffer,
                     gTextTable[gUnknown_085D3DD0[gPlayers[proc->unk66].co]
                                           .power[2].powerNameId],
                     0x8000, 0);
    }
    BG_EnableSyncBG0();
}
asm(".global sub_080852A8\n.thumb_set sub_080852A8, CoInfoScreen_DrawPageText\n");
