#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08054BA0.
 * sub_08054BA0 @ 0x08054BA0, sub_08054C04 @ 0x08054C04
 */

/* gUnknown_08136148 is an agbcc .rodata address constant holding 0x03004538
 * (checked against baserom.gba), not an object in ROM, so the three reads name
 * gUnknown_03004538 directly and agbcc re-emits the pool word.
 *
 * The two wrap steps are plain if/else with a store in BOTH arms, even though
 * the ROM has only one `strh`: cross-jumping merges the pair. Neither shorter
 * spelling works. A `?:` gives one store but agbcc canonicalises
 * `x == 0 ? 8 : x - 1` into `x != 0 ? x - 1 : 8` and emits the decrement as the
 * fall-through, which is the opposite block order from the ROM; hoisting the
 * value into a `u16` local instead forces a `lsls #16; lsrs #16` truncation on
 * the increment and pushes gUnknown_03004524's address into the .rodata pool
 * (+16 bytes). */
void MoveBattleAnimParamCursor(void)
{
    if (gUnknown_03004538 & 0x40)
    {
        if (gUnknown_03004524 == 0)
            gUnknown_03004524 = 8;
        else
            gUnknown_03004524--;
    }
    if (gUnknown_03004538 & 0x80)
    {
        if (gUnknown_03004524 == 8)
            gUnknown_03004524 = 0;
        else
            gUnknown_03004524++;
    }
    if (gUnknown_03004538 & 0x30)
        gUnknown_03004514 ^= 1;
}
asm(".global sub_08054BA0\n.thumb_set sub_08054BA0, MoveBattleAnimParamCursor\n");

/* The two flag tests are NESTED IFS, not `bit0 && bit1` -- both bits live in the
 * same byte, so agbcc's fold_truthop would merge an `&&` into one
 * `movs r0,#3; ands; cmp r0,#3`. Same reading as RunBattleAnimStepHandler. */
void LoadBattleAnimScene(void)
{
    SetUpBattleBackdrops(gUnknown_03004580[0][3], gUnknown_03004580[1][3]);
    if (gUnknown_03004504.bit0)
    {
        if (gUnknown_03004504.bit1)
            InitBattleHud();
    }
    else
    {
        InitBattleHud();
    }
    LoadFigures();
    CpuFastSet(gUnknown_08551A04, (void *)0x06002800, 0x200);
    sub_08054C5C();
}
asm(".global sub_08054C04\n.thumb_set sub_08054C04, LoadBattleAnimScene\n");
