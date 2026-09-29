#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052270.
 * sub_08052270 @ 0x08052270, sub_08052358 @ 0x08052358
 */

/* MATCHED. DebrisEffect_Loop plus a tail that fires once, on the last frame
 * (`unk28 == 8`): hand the slot's sprite id to SetSlotSpriteScaleX / SetSlotSpriteScaleY.
 * Read DebrisEffect_Loop's comment first; everything above the tail is identical.
 *
 * `e` is LOAD-BEARING and it is the whole cost of this function. Written inline
 * as `SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], gUnknown_08553B10[a])` agbcc
 * expands argument 1's address, loads it, then does the same for argument 2;
 * the ROM expands gUnknown_08553B10's address FIRST and issues the two loads in
 * the opposite order. Binding the second argument to a local moves the address
 * expansion to the binding statement, and `combine` then folds the load itself
 * back down to the call site -- so the `ldrsh` immediately before the `bl` is
 * NOT evidence against a local. `u16 e`, `s16 e` and a `u16 *` bound to
 * `&gUnknown_08553B10[a]` are byte-identical; only the inline form differs.
 * See the wave-20 section in docs/agbcc-codegen.md. The r4/r6 swap in the head
 * that goes with it is a consequence of this one edit, not a second fact. */
void AirBlastEffect_Loop(void)
{
    u16 a;
    u16 b;
    u16 dx;
    u16 x;
    u16 y;
    u16 e;

    gUnknown_03001470[gUnknown_03001FBC].unk28++;

    a = gUnknown_03001470[gUnknown_03001FBC].unk30;
    b = gUnknown_03001470[gUnknown_03001FBC].unk34;
    dx = gUnknown_03001470[gUnknown_03001FBC].unk28 * 4;

    if (a == 0)
        dx = -dx;

    x = gUnknown_02029A10[a].entries[b].x + gUnknown_08552D80[3].unk02[a] + dx;
    y = gUnknown_02029A10[a].entries[b].y;

    SetEffectScreenPosition(a, gUnknown_03001FBC, x, y);

    if (gUnknown_03001470[gUnknown_03001FBC].unk28 == 8)
    {
        e = gUnknown_08553B10[a];
        SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
        SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x180);
    }
}
asm(".global sub_08052270\n.thumb_set sub_08052270, AirBlastEffect_Loop\n");

/* MATCHED, first draft, one attempt. The slide-in animation tick that goes with
 * the matched AirBlastEffect_Init: bump the slot's frame counter, then re-place the
 * sprite at an x offset proportional to it, mirrored for side 0.
 *
 * It is `AirBlastEffect_Loop` minus that function's tail, and both are reached only as
 * slot 2 of an animation descriptor (gUnknown_085536D4 and gUnknown_085536BC),
 * which is why neither has a `bl` caller anywhere in asm/.
 *
 *  - `dx` is `(u16)(unk28 * 4)`. Read the `lsls #0x12; lsrs #0x10` pair as
 *    `(u32)x << 18 >> 16`, i.e. a net left shift of two truncated to 16 bits --
 *    NOT a mask plus a shift. The negation arm is the same u16 arithmetic
 *    (`rsbs; lsls #0x10; lsrs #0x10`).
 *  - The entry term is written FIRST in the x sum, for the reason
 *    src/decomp/c_08052154.c gives: agbcc expands the addresses in source order
 *    and issues the loads in the opposite one, so naming gUnknown_02029A10
 *    first is what puts its pool word ahead of gUnknown_08552D80's.
 *  - `gUnknown_08552D80[3]` is the constant row c_08052154.c identifies, folded
 *    to `adds rB, #0x56` (3 * 0x1c + offsetof unk02) on the base.
 *  - gUnknown_03001FBC is re-read after the `unk28++` because the word store
 *    can alias it; that costs nothing and needs no local. */
void DebrisEffect_Loop(void)
{
    u16 a;
    u16 b;
    u16 dx;
    u16 x;
    u16 y;

    gUnknown_03001470[gUnknown_03001FBC].unk28++;

    a = gUnknown_03001470[gUnknown_03001FBC].unk30;
    b = gUnknown_03001470[gUnknown_03001FBC].unk34;
    dx = gUnknown_03001470[gUnknown_03001FBC].unk28 * 4;

    if (a == 0)
        dx = -dx;

    x = gUnknown_02029A10[a].entries[b].x + gUnknown_08552D80[3].unk02[a] + dx;
    y = gUnknown_02029A10[a].entries[b].y;

    SetEffectScreenPosition(a, gUnknown_03001FBC, x, y);
}
asm(".global sub_08052358\n.thumb_set sub_08052358, DebrisEffect_Loop\n");
