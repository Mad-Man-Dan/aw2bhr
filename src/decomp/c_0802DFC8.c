#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DFC8.
 * sub_0802DFC8 @ 0x0802DFC8
 */

#include "hardware.h"

/* A frame step guarded on a key. The `lsls #0x10; lsrs #0x10` after the mask
 * is a u16 LOCAL, not a stray cast: gcc keeps the value in r4 across three
 * calls and then reuses it as the `strh` source, which is only worth doing
 * because the same object is both the tested value and the stored one. Writing
 * the store as a literal 0 loses that.
 *
 * The key is read at gpKeySt->held, offset 0, not `held` at +4. */

void MapCursorState_RangeWhileBHeld(void)
{
    u16 v;

    HandleMoveMapCursor();
    HandleMoveMapCursorInMoveRange();
    HandleMoveCameraWithMapCursor(4);
    StepMapCursorAndDraw(1);

    v = gpKeySt->held & 2;

    if (v == 0)
    {
        EndActiveMoveSlide();
        RebuildMapUnitLayers();
        HideRangeOverlay();

        gUnknown_03003334 = v;
    }
}
asm(".global sub_0802DFC8\n.thumb_set sub_0802DFC8, MapCursorState_RangeWhileBHeld\n");
