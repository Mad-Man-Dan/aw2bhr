#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801820C.
 * sub_0801820C @ 0x0801820C
 */

/* A third stepper on the same slot counter, moving it by 4 and clamping at
 * zero, then driving three display shadows from it and re-arming the
 * sub_08017EEC vblank handler.
 *
 * `lsls #0x10; cmp #0; bgt` is a SIGNED 16-bit test of the value already in a
 * register -- agbcc's way of comparing an s16 against zero without an `asr` --
 * which is the third independent reading of unk0e as signed. The two volatile
 * shadows take one CSE'd result; only the halfword load of unk0e is shared,
 * and the ROM shares it too. */
void CoScreenWipe_Step(struct Unk0200C528 *slot)
{
    slot->unk0e -= 4;
    if (slot->unk0e <= 0)
    {
        slot->unk08 = NULL;
        slot->unk0e = 0;
    }

    gUnknown_0300309C = -0x70 - slot->unk0e;
    gUnknown_03002028 = -0x70 - slot->unk0e;
    gUnknown_03001420 = slot->unk0e;
    QueueVBlankCallback((void *)sub_08017EEC);
}
asm(".global sub_0801820C\n.thumb_set sub_0801820C, CoScreenWipe_Step\n");
