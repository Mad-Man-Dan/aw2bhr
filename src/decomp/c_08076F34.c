#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076F34.
 * sub_08076F34 @ 0x08076F34
 */

#include "proc.h"

/* Walks the same 12-byte record list as WorldMapMarkers_Draw (see gUnknown_0202FE38 in
 * include/unknown-globals.h) looking for the record the camera is currently
 * closing on. The record base and the struct base are ONE symbol: the ROM
 * materialises 0x0202FE38 in a single pool word and derives 0x0202FDFC with
 * `adds/subs #0x3c`.
 *
 * `dx` and `dy` are s16 OBJECTS, not casts. The ROM keeps `value << 16` in a
 * callee-saved register across both calls and re-extracts with `asr #0x10` at
 * the IsWorldMapMarkerNearCursor call and `asr #0x12` for `>> 2` -- an unextended HImode
 * local extended at the use, per the W51-P chapter. An `int` local would
 * narrow once at the assignment and read out free thereafter.
 *
 * The two `+` operands read out BACKWARDS: `unk04 + unk00` is what emits
 * `ldrh [r6]` then `ldrh [r6,#4]`, because the right operand of the sum
 * expands first.
 *
 * The fifth argument to StartDifficultyStars is a literal 0 in both arms; the ROM
 * spends no instruction on it because each arm already has a register known to
 * hold zero (unk10 in the then arm, IsHardCampaignMode's result in the else arm). */

void SnapWorldMapCursorToMarker(ProcPtr proc)
{
    struct Unk0202FE38 *p;
    s16 dx;
    s16 dy;

    for (p = (struct Unk0202FE38 *)&gUnknown_0202FDFC.unk3c; p->unk00 != -1; p++)
    {
        dx = p->unk02 - (gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00);
        dy = p->unk04 - (gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02);

        if (IsWorldMapMarkerNearCursor(dx, dy))
        {
            MoveWorldMapCursorX(dx >> 2);
            MoveWorldMapCursorY(dy >> 2);

            if ((dx >> 2) != 0)
                return;

            if ((dy >> 2) != 0)
                return;

            if (gUnknown_0202FDFC.unk10 != 0)
                return;

            gUnknown_0202FDFC.unk10 = 1;
            gUnknown_0202FDFC.unk0c = p->unk00;

            MoveWorldMapSelectionFrame(p->unk02 - gUnknown_0202FDFC.unk00 + 1,
                         p->unk04 - gUnknown_0202FDFC.unk02 + 2,
                         1);

            if (IsHardCampaignMode())
                StartDifficultyStars(proc, gUnknown_08615194[p->unk00].hardModeStars,
                             gUnknown_0202FDFC.unk04 + 8,
                             gUnknown_0202FDFC.unk06 + 0xc, 0);
            else
                StartDifficultyStars(proc, gUnknown_08615194[p->unk00].difficultyStars,
                             gUnknown_0202FDFC.unk04 + 8,
                             gUnknown_0202FDFC.unk06 + 0xc, 0);

            return;
        }
    }
}
asm(".global sub_08076F34\n.thumb_set sub_08076F34, SnapWorldMapCursorToMarker\n");
