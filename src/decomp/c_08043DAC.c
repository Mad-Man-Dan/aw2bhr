#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043DAC.
 * sub_08043DAC @ 0x08043DAC, sub_08043DF4 @ 0x08043DF4, sub_08043E18 @ 0x08043E18
 */

/* Three things here are not free choices.
 *
 * `case 0:` sharing the `default:` label is what puts the `cmp #1; ble` low-
 * bound test into the decision tree.  With only cases 1 and 2 agbcc's
 * emit_case_nodes takes its "omit the branch to default if it avoids only one
 * right child" shortcut and emits two bare equality tests -- 4 bytes short.
 * A third case value is what makes the root's right subtree worth a bound
 * test, and 0 is the only value that leaves the root at 1 rather than at 2.
 *
 * The arm ORDER is source order: default first (it is the fall-through, with
 * no `b` in front of it), then case 1, then case 2.
 *
 * The return type is `int` with no `return` statement anywhere -- see the note
 * on the declaration.  Declared `void` the epilogue becomes `pop {r0}; bx r0`
 * instead of `pop {r1}; bx r1`, which is the last 2 bytes.
 *
 * The global is subscripted directly rather than bound to a `p` local; the
 * local spelling schedules the `ldr r1, [r1]` deref after the index arithmetic
 * instead of before it and swaps r0/r1 throughout.
 */
int PlayArmyCoMusic(u8 a)
{
    switch (gPlayers[a].coMode)
    {
    case 0:
    default:
        PlayMusicIfEnabled((s16)GetCoMusic(a));
        break;
    case 1:
        PlayCoPowerMusic(gPlayers[a].co);
        break;
    case 2:
        PlaySuperCoPowerMusic(gPlayers[a].co);
        break;
    }
}
asm(".global sub_08043DAC\n.thumb_set sub_08043DAC, PlayArmyCoMusic\n");

/* The parameter is dead inside the body -- see the note on the declaration.
 * The bare `lsls r0, r0, #0x18` in front of the test is what retyped
 * IsBlackHoleCo's return from `int` to `bool8` (wave 28, W28-B).
 */
void PlayCoPowerMusic(int a)
{
    if (IsBlackHoleCo(a))
        PlayMusicIfEnabled(0x1a3);
    else
        PlayMusicIfEnabled(0x1a4);
}
asm(".global sub_08043DF4\n.thumb_set sub_08043DF4, PlayCoPowerMusic\n");

void PlaySuperCoPowerMusic(int a)
{
    if (IsBlackHoleCo(a))
        PlayMusicIfEnabled(0x199);
    else
        PlayMusicIfEnabled(0x19a);
}
asm(".global sub_08043E18\n.thumb_set sub_08043E18, PlaySuperCoPowerMusic\n");
