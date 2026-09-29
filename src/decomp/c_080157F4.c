#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080157F4.
 * sub_080157F4 @ 0x080157F4
 */

/* Parameter two is `s16`, not the `u16` this file first declared -- the same
 * argument as on its twin SetSlotSpriteScaleX, from the same two callers. The bare
 * `strh` setter cannot distinguish int/u32/u16/s16, so the caller's
 * `lsls #0x10; asrs #0x10` is the only evidence there is. This function's own
 * bytes are unchanged -- re-verified with trymatch. */
void SetSlotSpriteScaleY(s16 a, s16 b)
{
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3e = b;
}
asm(".global sub_080157F4\n.thumb_set sub_080157F4, SetSlotSpriteScaleY\n");
