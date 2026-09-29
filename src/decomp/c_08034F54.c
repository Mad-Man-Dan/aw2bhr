#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034F54.
 * UnlockUnitSelection @ 0x08034F54, GetUnitSelectionLock @ 0x08034F60
 *
 * INFERRED names -- see the gGameLock.unitSelection note in include/lock.h.
 * LockUnitSelection is in c_08034F48.c. The old sub_XXXXXXXX symbols are kept
 * as linker aliases below so every other unit keeps resolving them unchanged.
 */

void UnlockUnitSelection(void)
{
    gGameLock.unitSelection = 0;
}

/* u8 and not int: MapCursorState_UnitMenuOpen and MapCursorState_UnitsTranslucent re-narrow the result with
 * `lsls #0x18` before testing it, and the body is a single `ldrb`. */
u8 GetUnitSelectionLock(void)
{
    return gGameLock.unitSelection;
}

asm(".global sub_08034F54\n.thumb_set sub_08034F54, UnlockUnitSelection\n"
    ".global sub_08034F60\n.thumb_set sub_08034F60, GetUnitSelectionLock\n");
