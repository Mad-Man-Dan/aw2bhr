#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005B24.
 * sub_08005B24 @ 0x08005B24
 */

/*
 * DesignRoomHelp_Loop -- run one frame of the two-page help screen.
 *
 * gActiveMap->state names the page: 0 and 1 are the first page, 0xA and 0xB
 * the second, and 0x5A means leave. A, B or SELECT at any point jumps straight
 * to 0x5A.
 *
 *   states 0 and 0xA draw their page (DesignRoomDrawHelpPage1, DesignRoomDrawHelpPage2) and fall
 *     into the matching key state on the same frame.
 *   state 1 waits for DOWN and state 0xB for UP: the other page is selected,
 *     the BG0 window is blanked and sound 0x67 plays. State 0xB also draws the
 *     five sprites that belong to the second page.
 *   state 0x5A tears the screen down -- both tilemaps blanked, BG0 and BG2
 *     flagged for copying to VRAM -- and hands gUnknown_03001FBC, the screen
 *     to return to, to ClearSlotScriptCallback. Leaving with anything other than B also
 *     sets gUnknown_03002F1C.
 *
 * The first frame after the mode change clears the state, zeroes the two
 * view-offset globals and opens the window with DrawWindowBackgroundOnBg2.
 *
 * global.h does not include hardware.h, so the include below is needed for
 * gpKeySt and the key names.
 */

#include "hardware.h"

void DesignRoomHelp_Loop(void)
{
    if (gActiveMap->stateChanged != 0)
    {
        gActiveMap->stateChanged = 0;
        gActiveMap->state = 0;
        gUnknown_03001418 = 0;
        gUnknown_03001FF8 = 0;
        DrawWindowBackgroundOnBg2(2, 2, 0x1A, 0xF);
        InitTextTileCache(0);
    }

    if ((gpKeySt->pressed & 7) != 0)
        gActiveMap->state = 0x5A;

    switch (gActiveMap->state)
    {
    case 0:
        InitTextTileCache(0);
        gActiveMap->state++;
        DesignRoomDrawHelpPage1();
        sub_08005EF0(1);
        /* fallthrough */
    case 1:
        if ((gpKeySt->pressed & DPAD_DOWN) != 0)
        {
            gActiveMap->state = 0xA;
            FillTilemapRect(gBG0TilemapBuffer, 0, 0, 0x1E, 0x14, 0);
            BG_EnableSyncBG0();
            sub_08005F1C();
            PlayMusicOrSfx2(0x67);
        }
        break;
    case 0xA:
        InitTextTileCache(0);
        gActiveMap->state++;
        DesignRoomDrawHelpPage2();
        sub_08005EF0(0);
        /* fallthrough */
    case 0xB:
        if ((gpKeySt->pressed & DPAD_UP) != 0)
        {
            gActiveMap->state = 0;
            FillTilemapRect(gBG0TilemapBuffer, 0, 0, 0x1E, 0x14, 0);
            BG_EnableSyncBG0();
            sub_08005F1C();
            PlayMusicOrSfx2(0x67);
        }
        DrawOamObject(0x35, 0x28, 0x421, 0, 0);
        DrawOamObject(0x36, 0x28, 0x431, 0, 0);
        DrawOamObject(0x37, 0x28, 0x441, 0, 0);
        DrawOamObject(0x38, 0x28, 0x451, 0, 0);
        DrawOamObject(0x3C, 0x20, 0x461, 0, 0);
        break;
    }

    if (gActiveMap->state == 0x5A)
    {
        if ((gpKeySt->pressed & 2) == 0)
            gUnknown_03002F1C = 1;
        sub_08005F1C();
        FillTilemapRect(gBG0TilemapBuffer, 0, 0, 0x1E, 0x14, 0);
        FillTilemapRect(gBG2TilemapBuffer, 0, 0, 0x1E, 0x14, 0x360);
        BG_EnableSyncBG0();
        BG_EnableSyncBG2();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_08005B24\n.thumb_set sub_08005B24, DesignRoomHelp_Loop\n");
