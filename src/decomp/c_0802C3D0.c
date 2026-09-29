#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C3D0.
 * sub_0802C3D0 @ 0x0802C3D0
 */

#include "hardware.h"

void MinimapScreen_Loop(void)
{
    HandleMoveMapCursor();
    MoveMapCursorFromHeldKeys();
    HandleMoveCameraWithMapCursor(8);

    if (gMap->unk10 & 0xf)
        return;

    if (*(u32 *)&gUnknown_030033E0 & 0x00070007)
        return;

    if (!(gpKeySt->pressed & (A_BUTTON | B_BUTTON | START_BUTTON)))
        return;

    PlayMusicOrSfx2(0x66);
    ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_0802C3D0\n.thumb_set sub_0802C3D0, MinimapScreen_Loop\n");
