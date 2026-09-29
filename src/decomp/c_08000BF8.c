#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000BF8.
 * sub_08000BF8 @ 0x08000BF8, sub_08000C68 @ 0x08000C68
 */

/*
 * sub_08000BF8 -- flag that the selection and the tile under the cursor differ.
 *
 * Compares what the player has selected with what is under the cursor -- the
 * terrain id in terrain mode, the unit type in unit mode -- and sets
 * gActiveMap->cursorMoved when they are not the same. DesignRoomUpdateSelectionPanel reads that
 * flag.
 *
 * DesignRoomPickUnderCursor below does the opposite: it plays sound 0x65 and makes whatever
 * is under the cursor the new selection (DesignRoomSelectItem).
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - The terrain byte must be reached as `gMap->terrain[x + gMap->rowOffset
 *     [y]]`, not by byte arithmetic on a u8 pointer. The compiler folds the
 *     two struct offsets into the index differently and the output changes.
 */

void sub_08000BF8(void)
{
    if (gActiveMap->editMode == 0)
    {
        if (gActiveMap->selectedTerrain
            != gMap->terrain[
                   gActiveMap->cursorX
                   + gMap->rowOffset[gActiveMap->cursorY]])
            gActiveMap->cursorMoved = 1;
    }
    else
    {
        if (gActiveMap->cursorUnit
            != GetUnitTypeAt(gActiveMap->cursorX, gActiveMap->cursorY))
            gActiveMap->cursorMoved = 1;
    }
}

void DesignRoomPickUnderCursor(void)
{
    PlayMusicOrSfx2(0x65);

    if (gActiveMap->editMode == 0)
        DesignRoomSelectItem(gMap->terrain[
            gActiveMap->cursorX
            + gMap->rowOffset[gActiveMap->cursorY]]);
    else
        DesignRoomSelectItem(GetUnitTypeAt(gActiveMap->cursorX, gActiveMap->cursorY));
}
asm(".global sub_08000C68\n.thumb_set sub_08000C68, DesignRoomPickUnderCursor\n");
