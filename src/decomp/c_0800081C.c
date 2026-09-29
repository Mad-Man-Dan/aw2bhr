#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800081C.
 * sub_0800081C @ 0x0800081C
 */

#include "hardware.h"
#define MAP gMap

/*
 * DesignRoomMode_Paint -- run one frame of the map editor's cursor and painting mode.
 *
 * Reads the pad, works out whether the current selection may be placed under
 * the cursor, draws the cursor accordingly and acts on the buttons.
 * gUnknown_030033E4 holds the live cursor position (unk00 = x, unk02 = y);
 * gActiveMap keeps the copy this function last saw.
 *
 *   - First frame after the mode change: copy the cursor position, rebuild the
 *     editor's view, and return through state 0 without reading the pad.
 *   - Pad source: while the cursor is still moving (the live position differs
 *     from the stored one) held keys are used, otherwise newly pressed keys.
 *   - Placement test, result in r: 1 allowed, 6 refused, 5 for unit id 0x19.
 *     In terrain mode each of terrain 2, 5, 0xC, 0xD, 0x10 and 0x13 has its
 *     own test, and flag 0x2000 refuses everything. In unit mode the unit's
 *     movement type is charged for this terrain and a cost of -1 refuses the
 *     tile. StepMapCursorAndDraw then draws the cursor for r.
 *   - A: in terrain mode paint the tile (MakeTile) unless refused; in unit
 *     mode ask DesignRoomPlaceUnitAtCursor and place the unit when it answers 1, setting flag
 *     0x1000 on any positive answer. A refused press plays sound 0x68 and
 *     counts cursorIdleFrames up; after 0x31 of them it calls sub_08004D10.
 *   - B: redraw the ring and re-test the tile under the cursor.
 *   - START selects mode 5; SELECT mode 3; R mode 2 in terrain mode; L mode 2
 *     in unit mode. All four need IsMapCursorSettled true, and the last three also
 *     need GetMapLock to be 0. When none of them is pressed the cursor
 *     position is resynchronised.
 *   - Last: play gActiveMap->soundId if it was set, otherwise the id
 *     DesignRoomHandleCursorInput returned, and count inputDelay down, clearing flag 0x2000
 *     when it reaches zero.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The movement-cost lookup must build its index with a separate
 *     `index +=` and choose between 1 and 6 with `?:`. Spelled as one
 *     expression, or as an if/else, the compiler picks different registers.
 *   - `gActiveMap->editMode = t;` and `cursorIdleTimer = t;` store a zero that
 *     is known to be in `t`; the original reuses the register the key test
 *     left behind, so a literal 0 no longer matches.
 */
void DesignRoomMode_Paint(void)
{
    int r;
    int v;
    int keys;
    int k;
    int m;
    int index;
    u16 t;

    if (gActiveMap->stateChanged != 0)
    {
        gActiveMap->stateChanged = 0;
        gActiveMap->state = 0;
        gActiveMap->cursorX = gUnknown_030033E4.unk00;
        gActiveMap->cursorY = gUnknown_030033E4.unk02;
        sub_08002E3C();
        sub_08002E5C();
        sub_08002D7C();
        sub_080059E4();
        DesignRoomShowTilePanel();
        DesignRoomShowCoordBox();
        DesignRoomCountArmyUnits();
    }

    if (gActiveMap->state == 0)
    {
        gActiveMap->state = 1;
        return;
    }

    gActiveMap->soundId = 0;
    HandleMoveMapCursor();
    v = DesignRoomHandleCursorInput();
    HandleMoveCameraWithMapCursor(4);

    if (gUnknown_030033E4.unk00 != gActiveMap->cursorX
     || gUnknown_030033E4.unk02 != gActiveMap->cursorY)
    {
        keys = gpKeySt->held;
        gActiveMap->cursorX = gUnknown_030033E4.unk00;
        gActiveMap->cursorY = gUnknown_030033E4.unk02;
    }
    else
    {
        keys = gpKeySt->pressed;
    }

    if (gActiveMap->editMode == 0)
    {
        r = 1;
        if (gActiveMap->selectedTerrain == 0xd)
        {
            if (sub_0800B528(gActiveMap->cursorX, gActiveMap->cursorY) < 0
             || GetShoalTile(gActiveMap->cursorX, gActiveMap->cursorY) < 0)
                r = 6;
        }
        else if (gActiveMap->selectedTerrain == 2)
        {
            if (CanPlaceRiverAt(gActiveMap->cursorX, gActiveMap->cursorY) == 0)
                r = 6;
        }
        else if (gActiveMap->selectedTerrain == 0x13)
        {
            if (CanPlaceReefAt(gActiveMap->cursorX, gActiveMap->cursorY) == 0)
                r = 6;
        }
        else if (gActiveMap->selectedTerrain == 0xc)
        {
            if (CanPlaceBridgeAt(gActiveMap->cursorX, gActiveMap->cursorY) == 0)
                r = 6;
        }
        else if (gActiveMap->selectedTerrain == 5)
        {
            if (MAP->terrain[MAP->rowOffset[gActiveMap->cursorY]
                             + gActiveMap->cursorX] == 2)
            {
                if (CanPlaceBridgeAt(gActiveMap->cursorX, gActiveMap->cursorY) == 0)
                    r = 6;
            }
        }
        else if (gActiveMap->selectedTerrain == 0x10)
        {
            if (sub_08010DD4(gActiveMap->cursorX, gActiveMap->cursorY) != 0)
                r = 6;
        }
        else if (gActiveMap->flags & 0x2000)
        {
            r = 6;
        }
    }
    else if (gActiveMap->cursorUnit == 0x19)
    {
        r = 5;
    }
    else
    {
        s8 *tbl = gUnknown_085D3DD0[1].power[0].movementChart[0];

        index = MAP->terrain[MAP->rowOffset[gActiveMap->cursorY]
                             + gActiveMap->cursorX] & 0x1f;
        index += gUnknown_085D5ABC[gActiveMap->cursorUnit & 0x3f].movementType * 32;
        r = tbl[index] != -1 ? 1 : 6;
    }

    StepMapCursorAndDraw(r);

    if (keys & 1)
    {
        if (gActiveMap->editMode == 0)
        {
            if (r != 6)
            {
                MakeTile();
                RenderMap();
                RebuildMapUnitLayers2();
            }
        }
        else
        {
            k = DesignRoomPlaceUnitAtCursor();
            if (k == 1)
            {
                gActiveMap->soundId = 0;
                v = 0;
                sub_08035850(gActiveMap->cursorX, gActiveMap->cursorY,
                             gActiveMap->cursorUnit & 0x3f);
            }
            if (k > 0)
                gActiveMap->flags |= 0x1000;
        }

        if (r == 6)
        {
            if (gpKeySt->pressed & 1)
                PlayMusicOrSfx2(0x68);

            gActiveMap->cursorIdleFrames++;
            gActiveMap->cursorIdleTimer = 0xc;
            if (gActiveMap->cursorIdleFrames > 0x31)
            {
                gActiveMap->cursorIdleFrames = 0;
                sub_08004D10();
            }
        }
    }
    else
    {
        t = gpKeySt->pressed & 2;
        if (t != 0)
        {
            DesignRoomCountArmyUnits();
            sub_08000BF8();
        }
        else if (gActiveMap->cursorIdleTimer-- <= 0)
        {
            gActiveMap->cursorIdleTimer = t;
            gActiveMap->cursorIdleFrames = t;
        }
    }

    if (IsMapCursorSettled() && (keys & 8))
        DesignRoomSetMode(5);

    if (gActiveMap->soundId != 0)
        PlayMusicOrSfx2(gActiveMap->soundId);
    else if (v != 0)
        PlayMusicOrSfx2((s16)v);

    if (gActiveMap->inputDelay > 0)
    {
        gActiveMap->inputDelay--;
        if (gActiveMap->inputDelay == 0)
            gActiveMap->flags &= 0xDFFF;
    }

    if (IsMapCursorSettled() && GetMapLock() == 0)
    {
        t = gpKeySt->pressed & 4;
        if (t != 0)
        {
            DesignRoomSetMode(3);
        }
        else
        {
            m = gpKeySt->pressed & (A_BUTTON | B_BUTTON | R_BUTTON | L_BUTTON);
            if (m == 0x100)
            {
                gActiveMap->editMode = t;
                DesignRoomSetMode(2);
            }
            else if (m == 0x200)
            {
                gActiveMap->editMode = 1;
                DesignRoomSetMode(2);
            }
            else
            {
                gActiveMap->cursorX = gUnknown_030033E4.unk00;
                gActiveMap->cursorY = gUnknown_030033E4.unk02;
            }
        }
    }
}
asm(".global sub_0800081C\n.thumb_set sub_0800081C, DesignRoomMode_Paint\n");
