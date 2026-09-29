#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080157A4.
 * sub_080157A4 @ 0x080157A4
 */

/* Parameter two is `s16`, not the `u16` this file first declared. The bare
 * `strh` setter cannot distinguish int/u32/u16/s16 (see the setter table in
 * docs/agbcc-codegen.md), so the callee is silent and the CALLER settles it:
 * DeathHandler_Tank and DeathHandler_Air both bind a u16 table element to a local and
 * sign-extend it `lsls #0x10; asrs #0x10` immediately before the call, which
 * is the `u16 -> s16` conversion and is two instructions of real code. A `u16`
 * parameter emits nothing there and leaves those two callers short. This
 * function's own bytes are unchanged -- re-verified with trymatch. */
void SetSlotSpriteScaleX(s16 a, s16 b)
{
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3c = b;
}
asm(".global sub_080157A4\n.thumb_set sub_080157A4, SetSlotSpriteScaleX\n");
