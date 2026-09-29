#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000694.
 * sub_08000694 @ 0x08000694
 */

#include "hardware.h"

/*
 * sub_08000694 -- run one frame of the mode that fades a screen in, waits for
 * a button, fades back out and returns to mode 1.
 *
 * Driven by gActiveMap->state. The first frame after the mode change hides the
 * map (DesignRoomHideTilePanel, DesignRoomHideCoordBox, sub_080039D0), arms a two-frame timer and
 * plays sound 0x76. gUnknown_03001FFC is the fade level the screen waits on.
 *
 *   state 0:   count the timer down, then go to state 50.
 *   state 50:  set the two view-offset globals to -40 and -60, fall into 60.
 *   state 60:  once the fade level is past 5, start the screen with
 *              DrawDesignRoomMapPreview and go to state 70.
 *   state 70:  run the screen each frame; A, B or START arms a 10-frame timer
 *              and goes to state 80.
 *   state 80:  when the timer runs out, close the screen, blank the 15 x 10
 *              tile window at the top left of BG0, flag BG0 for copying to
 *              VRAM and play sound 0x66.
 *   state 90:  once the fade level is back to 0, clear the mosaic bit in the
 *              BG0 and BG2 control words, store 0xA0 in gUnknown_03002EFC and
 *              zero gUnknown_030030C4's low byte (both purposes unknown), then
 *              call RebuildMapUnitLayers2 and go to state 100.
 *   state 100: bring the map back (DesignRoomShowTilePanel, DesignRoomShowCoordBox), switch to mode
 *              1 and leave the state at 40, which no case here handles.
 */

void sub_08000694(void)
{
    if (gActiveMap->stateChanged != 0)
    {
        gActiveMap->stateChanged = 0;
        gActiveMap->state = 0;
        DesignRoomHideTilePanel();
        DesignRoomHideCoordBox();
        gActiveMap->stateTimer = 2;
        sub_080039D0();
        PlayMusicOrSfx2(0x76);
    }

    switch (gActiveMap->state)
    {
    case 0:
        if (--gActiveMap->stateTimer <= 0)
            gActiveMap->state = 50;
        break;

    case 50:
        gUnknown_03001418 = 0xFFD8;
        gUnknown_03001FF8 = 0xFFC4;
        gActiveMap->state = 60;
        /* fall through */
    case 60:
    {
        /* Read the fade level through an int first: comparing the u16 global
         * directly makes the compare unsigned, and the original compares
         * signed. */
        int blend = gUnknown_03001FFC;

        if (blend > 5)
        {
            gActiveMap->state = 70;
            DrawDesignRoomMapPreview(0, 0);
        }
        break;
    }

    case 70:
        HandleMoveMapCursor();
        MoveMapCursorFromHeldKeys();
        HandleMoveCameraWithMapCursor(8);
        if (gpKeySt->pressed & (A_BUTTON | B_BUTTON | START_BUTTON))
        {
            gActiveMap->state = 80;
            gActiveMap->stateTimer = 10;
        }
        break;

    case 80:
        if (--gActiveMap->stateTimer < 0)
        {
            gActiveMap->state = 90;
            EndMapPreviewEffects();
            FillTilemapRect(gBG0TilemapBuffer, 0, 0, 15, 10, 0);
            BG_EnableSyncBG0();
            sub_080039BC();
            PlayMusicOrSfx2(0x66);
        }
        break;

    case 90:
        if (gUnknown_03001FFC == 0)
        {
            gUnknown_03002EFC = 0xa0;
            gActiveMap->state = 100;
            gUnknown_03001FE8.bits.mosaic = 0;
            ((union BgCntBuf *)&gUnknown_0300251C)->bits.mosaic = 0;
            *(u8 *)&gUnknown_030030C4 = 0;
            RebuildMapUnitLayers2();
        }
        break;

    case 100:
        gActiveMap->state = 40;
        DesignRoomShowTilePanel();
        DesignRoomShowCoordBox();
        DesignRoomSetMode(1);
        break;
    }
}
