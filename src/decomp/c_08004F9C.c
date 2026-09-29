#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004F9C.
 * sub_08004F9C @ 0x08004F9C
 */

/*
 * DesignRoomSaveToSlot -- run the save-slot call for the current design and redraw
 * that slot's row on screen.
 *
 * SaveDesignRoomSlot is handed gActiveMap->designSlot, gActiveMap->designName and
 * sub_0800C9E8's result; whether it writes the slot or reads it is not visible
 * here. sub_0800CB30 brackets the call, first with (0, 0) and then with (1,
 * <what the first call returned>), so it suspends something and restores it
 * from that token.
 *
 * Slots 0, 1 and 2 occupy tile rows 7, 9 and 0xB. The row is blanked
 * (FillTilemapRect), given its marker (PutTilePoolGraphicTilemap) and has the name drawn on it
 * (PutTextScriptImmediate), then BG0 is flagged for copying to VRAM. Flag 0x100 means
 * the name is to be emptied, and is consumed here; flag 0x1000 is cleared on
 * every call.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `case 0: break;` must stay. It shares the switch's default label, which
 *     is what makes the compiler emit four comparisons instead of three.
 *   - The tilemap index stays bracketed as `+ (v * 32 + 3)`. Without the
 *     brackets the base address is added first and the shift-and-add pair the
 *     original uses does not come out.
 */

void DesignRoomSaveToSlot(void)
{
    int t;
    int v;

    t = sub_0800CB30(0, 0);
    SaveDesignRoomSlot(gActiveMap->designSlot, gActiveMap->designName,
                 sub_0800C9E8());
    sub_0800CB30(1, t);

    v = 7;
    switch ((s8)gActiveMap->designSlot)
    {
    case 0:
        break;
    case 1:
        v = 9;
        break;
    case 2:
        v = 0xB;
        break;
    }

    FillTilemapRect(gBG0TilemapBuffer, 3, v, 0xB, 2, 0);
    PutTilePoolGraphicTilemap(9, gBG0TilemapBuffer + (v * 32 + 3));
    PutTextScriptImmediate(5, (s16)v, gBG0TilemapBuffer, gActiveMap->designName,
                 0x8000, 0);
    BG_EnableSyncBG0();

    if (gActiveMap->flags & 0x100)
    {
        gActiveMap->flags &= 0xFEFF;
        gActiveMap->designName[0] = 0;
    }

    gActiveMap->flags &= 0xEFFF;
}
asm(".global sub_08004F9C\n.thumb_set sub_08004F9C, DesignRoomSaveToSlot\n");
