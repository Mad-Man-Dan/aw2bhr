#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E250.
 * sub_0802E250 @ 0x0802E250, sub_0802E260 @ 0x0802E260, sub_0802E278 @ 0x0802E278
 */

#include "hardware.h"

/* Two statements, no arguments to the second. OpenMinimap's own prologue
 * reads no argument register, so r0 still holding 0x76 at the second bl is a
 * leftover, not a hidden parameter. */
void MapCursor_OnPressStart(void)
{
    PlayMusicOrSfx2(0x76);
    OpenMinimap();
}
asm(".global sub_0802E250\n.thumb_set sub_0802E250, MapCursor_OnPressStart\n");

/* A four-call teardown. IsMapCursorSettled returns u8 and the result is discarded
 * here -- MapCursorState_UnitsTranslucent next door is this function with 8 instead of 4 and the
 * result actually tested, which is what fixes the return type. */
void sub_0802E260(void)
{
    HandleMoveMapCursor();
    MoveMapCursorFromHeldKeys();
    HandleMoveCameraWithMapCursor(4);
    IsMapCursorSettled();
}

/* sub_0802E260 with 8 for 4 and a guarded tail, and NOT a variant of it -- the
 * tail is the whole function.
 *
 * The masked key value is stored, not a literal 0: the ROM writes r4, the
 * register holding `keys & 2`, into gUnknown_03003334, and the branch above it
 * has already proved r4 is zero. A literal would have been `movs r0,#0`.
 *
 * The `lsls #0x10; lsrs #0x10` after the `ands` is the u16 local's own
 * truncation of the int the mask promotes to. */
void MapCursorState_UnitsTranslucent(void)
{
    u16 k;

    HandleMoveMapCursor();
    MoveMapCursorFromHeldKeys();
    HandleMoveCameraWithMapCursor(8);

    if (IsMapCursorSettled())
    {
        k = gpKeySt->held & 2;

        if (k == 0)
        {
            SetDefaultColorEffects();
            gUnknown_03003334 = k;
        }
    }
}
asm(".global sub_0802E278\n.thumb_set sub_0802E278, MapCursorState_UnitsTranslucent\n");
