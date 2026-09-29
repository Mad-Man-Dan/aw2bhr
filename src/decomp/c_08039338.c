#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039338.
 * sub_08039338 @ 0x08039338
 */

/* A halving approach to 0x100: each frame the stored x moves half the distance
 * to 0x100, and the frame it lands exactly one short it is snapped onto 0x100
 * and the arrival handlers run.
 *
 * `- 0x100` IS WRITTEN TWICE and that is what the register allocation records:
 * the constant is materialised once into r3 (it is too wide for an immediate)
 * and reused by the compare, so spelling the test `v == 0x101` instead costs a
 * second pool word and compares in the shifted domain -- measured, 0x1010000
 * against `asrs; adds r3; cmp #1`.
 *
 * `t` is int and `v` is s16: the `lsls #0x10 / lsrs #0x10 / asrs #0x10` triple
 * is one store of the narrowed value plus one sign-extended read of it in the
 * same breath, not two variables. */
void sub_08039338(void)
{
    int t;
    s16 v;

    t = (s16)GetSlotSpriteScaleX(gUnknown_03001FBC);
    v = t - (t - 0x100) / 2;

    if (v - 0x100 == 1)
        v = 0x100;

    SetSlotSpriteScaleX(gUnknown_03001FBC, v);
    SetSlotSpriteScaleY(gUnknown_03001FBC, v);

    if (v == 0x100)
    {
        DisableSlotSpriteAffine(gUnknown_03001FBC);
        SetSlotSpriteScriptIndex(gUnknown_03001FBC, 1);
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
