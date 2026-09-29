#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B264.
 * sub_0803B264 @ 0x0803B264, sub_0803B2BC @ 0x0803B2BC
 */

/* Nudges the gUnknown_03001FBC object's stored y by +0x10 and, unless its
 * x is the 0x80 sentinel, its x by -0x10.
 *
 * gUnknown_03001FBC IS RELOADED at every use -- five `movs rN,#0; ldrsh` off
 * one pool word -- because each call may change it. Passing it as both
 * arguments of ONE SetSlotSpriteRotation call makes gcc read it once before the inner
 * call and keep it in a callee-saved register instead, which is two
 * instructions different. Splitting the inner call into its own statement is
 * what restores the reload.
 *
 * `t` is `int`, not `s16`. With `s16` the +0x10 is applied before the shift
 * pair; with `int` the conversion at SetSlotSpriteRotation's `s16` parameter folds
 * with the callee's own re-narrowing into `lsls #0x10; adds 0x100000;
 * asrs #0x10` -- the ROM's form, and the same instruction count. */
void SlotSprite_SpinShrinkLoop(void)
{
    int t;
    s16 v;

    t = GetSlotSpriteRotation(gUnknown_03001FBC);
    SetSlotSpriteRotation(gUnknown_03001FBC, t + 0x10);

    v = GetSlotSpriteScaleX(gUnknown_03001FBC);

    if (v != 0x80)
    {
        v -= 0x10;
        SetSlotSpriteScaleX(gUnknown_03001FBC, v);
        SetSlotSpriteScaleY(gUnknown_03001FBC, v);
    }
}
asm(".global sub_0803B264\n.thumb_set sub_0803B264, SlotSprite_SpinShrinkLoop\n");

/* Creates a gUnknown_03001470 object, rewrites two fields of its OBJ attribute
 * pair and then runs the standard five-call setup on it.
 *
 * `struct UnkVec` moves BY VALUE through both accessors -- GetSlotSpriteAttrs
 * returns it (hidden buffer pointer in r0, hence the `add r0, sp, #4`) and
 * SetSlotSpriteAttrs takes it in r1/r2. The whole attribute edit is ONE expression:
 * the ROM never stores the intermediate back to the stack slot, which two
 * statements would have made it do.
 *
 * 0x4000 is `movs r3,#0x80; lsls r3,#7` and 0x200 is `movs r5,#0x80;
 * lsls r5,#2`, both literals agbcc rebuilt; the 0x200 is materialised once and
 * shared by the last two calls. */
void StartSpinningSlotSprite(void)
{
    struct UnkVec v;
    s8 i;

    i = StartSlotScriptWithSprite((void *)gUnknown_0849E700, 0, gUnknown_0849E6F8, 0, 0);

    v = GetSlotSpriteAttrs(i);
    v.unk04 = (((v.unk04 & 0xFFFF0FFF) | 0x4000) & 0xFFFFFC00) | 0x74;
    SetSlotSpriteAttrs(i, v);

    SetSlotSpritePosition(i, 0xaa, 0x20);
    EnableSlotSpriteAffine(i);
    SetSlotSpriteDoubleSize(i);
    SetSlotSpriteScaleX(i, 0x200);
    SetSlotSpriteScaleY(i, 0x200);
}
asm(".global sub_0803B2BC\n.thumb_set sub_0803B2BC, StartSpinningSlotSprite\n");
