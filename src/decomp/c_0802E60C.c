#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E60C.
 * sub_0802E60C @ 0x0802E60C
 */

void ConfirmUnitDestination(s16 a1, s16 a2)
{
    if (!IsValidMoveDestination(a1, a2))
    {
        PlayMusicOrSfx2(0x68);
        return;
    }

    HideRangeOverlay();

    gUnknown_03003F24.pos.unk00 = gUnknown_030040D8->unk02;
    gUnknown_03003F24.pos.unk02 = gUnknown_030040D8->unk03;
    gUnknown_03003100.pos.unk00 = a1;
    gUnknown_03003100.pos.unk02 = a2;

    UnlockUnitSelection();
    gUnknown_03003334 = 3;

    if (gUnknown_03003F24.raw == gUnknown_03003100.raw)
        return;

    EncodeMovePathDirections();

    if ((u8)TruncatePathAtHiddenEnemy(gUnknown_03003F24.spos.unk00, gUnknown_03003F24.spos.unk02,
            gUnknown_03003110, 4) == 1)
        gUnknown_03003334 = 5;

    StartUnitMoveSlide(gUnknown_03003110);
}
asm(".global sub_0802E60C\n.thumb_set sub_0802E60C, ConfirmUnitDestination\n");
