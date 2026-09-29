#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000CCC.
 * DesignRoomSelectItem @ 0x08000CCC
 */

/*
 * DesignRoomSelectItem -- make a1 the player's current selection and move the
 * on-screen pick ring to it.
 *
 * a1 is a terrain id in terrain mode, or a unit id with the army in bits 6-7
 * in unit mode. DesignRoomFindItemIndex turns the id into a position in that mode's list.
 *
 *   1. Store that position offset back by 4 entries (terrain) or 3 (unit),
 *      wrapped into 0..0x10 or 0..0x13.
 *   2. DesignRoomBuildRing redraws the ring around the new position.
 *   3. Read the id now sitting 4 slots on in gDesignRing (3 in unit mode),
 *      wrapping at the end of the 10-entry ring (8 entries in unit mode), and
 *      store it as the selection: selectedTerrain, or cursorUnit with a1's
 *      army bits kept and the id masked to six bits.
 *   4. Copy 0x460 bytes of graphics to 0x06014D40 in VRAM.
 *
 * In unit mode the id 0x19 leaves unitArmy alone; any other id sets it from
 * bits 6-7 plus one.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - In unit mode the two masked halves must be combined and assigned back
 *     into `b`, the same variable that is then stored. With extra locals, with
 *     `b |= a`, or with a separate result variable, the compiler reuses an
 *     operand's register instead of giving each half a fresh one.
 *   - `a -= 4;` stays its own statement after the call, and
 *     `b = gDesignRing[b].itemId;` stays its own statement before the store.
 *     Folding either into the expression next to it moves values into
 *     different registers.
 */

void DesignRoomSelectItem(int a1)
{
    int a;
    int b;

    if (gActiveMap->editMode == 0)
    {
        a = DesignRoomFindItemIndex(a1);
        a -= 4;
        if (a < 0)
            a += 0x11;
        gActiveMap->terrainListIndex = a;
        DesignRoomBuildRing(gActiveMap->editMode, a1);
        b = gActiveMap->ringIndex + 4;
        if (b > 9)
            b = gActiveMap->ringIndex - 6;
        b = gDesignRing[b].itemId;
        gActiveMap->selectedTerrain = b;
    }
    else
    {
        if (a1 != 0x19)
            gActiveMap->unitArmy = (a1 >> 6) + 1;
        a = DesignRoomFindItemIndex(a1);
        a -= 3;
        if (a < 0)
            a += 0x14;
        gActiveMap->unitListIndex = a;
        DesignRoomBuildRing(gActiveMap->editMode, a1);
        b = gActiveMap->ringIndex + 3;
        if (b > 7)
            b = gActiveMap->ringIndex - 5;
        b = gDesignRing[b].itemId;
        b = (a1 & 0xC0) | (b & 0x3F);
        gActiveMap->cursorUnit = b;
    }

    RegisterDataMove(gUnknown_0808D8AC, (void *)0x06014D40, 0x8C << 3);
}

asm(".global sub_08000CCC\n.thumb_set sub_08000CCC, DesignRoomSelectItem\n");
