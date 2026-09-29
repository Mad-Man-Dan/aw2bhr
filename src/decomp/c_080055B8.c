#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080055B8.
 * sub_080055B8 @ 0x080055B8, sub_08005634 @ 0x08005634, sub_080056B0 @ 0x080056B0
 */

/*
 * DesignRoomShowSlotPreview0 -- put save slot 0's name box on screen, or take it down.
 *
 * LoadDesignRoomName is asked about slot 0, with gDesignRoomName. Either way
 * EndMapPreviewEffects runs and the 0xF x 0xA tile window at (0xE, 4) of BG0 is
 * blanked. If the answer was not 1 the window is then flagged for copying to
 * VRAM and nothing more happens; if it was 1, DrawDesignRoomSlotPreview draws slot 0 into
 * the window and the two view-offset globals are zeroed instead.
 *
 * The three parameters are never read here. They are known from the only
 * caller, sub_08004D28, which passes three zeroes to each of the three
 * functions in this file.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - DesignRoomShowSlotPreview1 and DesignRoomShowSlotPreview2 are this function again for slots 1 and 2.
 *     The original holds all three copies; do not factor them into one.
 *   - Both arms of the `if` repeat the same first two calls. Merged into a
 *     common prologue, the zero stored on the stack ends up in a different
 *     register from the original's.
 */
void DesignRoomShowSlotPreview0(int a, int b, int c)
{
    if (LoadDesignRoomName(0, gDesignRoomName) != 1)
    {
        EndMapPreviewEffects();
        FillTilemapRect(gBG0TilemapBuffer, 0xE, 4, 0xF, 0xA, 0);
        BG_EnableSyncBG0();
    }
    else
    {
        EndMapPreviewEffects();
        FillTilemapRect(gBG0TilemapBuffer, 0xE, 4, 0xF, 0xA, 0);
        DrawDesignRoomSlotPreview(0xE, 4, 0);
        gUnknown_03001418 = 0;
        gUnknown_03001FF8 = 0;
    }
}
asm(".global sub_080055B8\n.thumb_set sub_080055B8, DesignRoomShowSlotPreview0\n");

/* DesignRoomShowSlotPreview1 -- the same as DesignRoomShowSlotPreview0, for save slot 1. */
void DesignRoomShowSlotPreview1(int a, int b, int c)
{
    if (LoadDesignRoomName(1, gDesignRoomName) != 1)
    {
        EndMapPreviewEffects();
        FillTilemapRect(gBG0TilemapBuffer, 0xE, 4, 0xF, 0xA, 0);
        BG_EnableSyncBG0();
    }
    else
    {
        EndMapPreviewEffects();
        FillTilemapRect(gBG0TilemapBuffer, 0xE, 4, 0xF, 0xA, 0);
        DrawDesignRoomSlotPreview(0xE, 4, 1);
        gUnknown_03001418 = 0;
        gUnknown_03001FF8 = 0;
    }
}
asm(".global sub_08005634\n.thumb_set sub_08005634, DesignRoomShowSlotPreview1\n");

/* DesignRoomShowSlotPreview2 -- the same as DesignRoomShowSlotPreview0, for save slot 2. */
void DesignRoomShowSlotPreview2(int a, int b, int c)
{
    if (LoadDesignRoomName(2, gDesignRoomName) != 1)
    {
        EndMapPreviewEffects();
        FillTilemapRect(gBG0TilemapBuffer, 0xE, 4, 0xF, 0xA, 0);
        BG_EnableSyncBG0();
    }
    else
    {
        EndMapPreviewEffects();
        FillTilemapRect(gBG0TilemapBuffer, 0xE, 4, 0xF, 0xA, 0);
        DrawDesignRoomSlotPreview(0xE, 4, 2);
        gUnknown_03001418 = 0;
        gUnknown_03001FF8 = 0;
    }
}
asm(".global sub_080056B0\n.thumb_set sub_080056B0, DesignRoomShowSlotPreview2\n");
