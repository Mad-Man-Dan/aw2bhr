#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080425B8.
 * sub_080425B8 @ 0x080425B8, sub_080425E0 @ 0x080425E0, sub_080425FC @ 0x080425FC, sub_08042618 @ 0x08042618, sub_08042634 @ 0x08042634
 */

/* gUnknown_030040D8 is re-loaded between the two stores: the `strb` through it
 * can alias the pointer global itself, so agbcc cannot keep it live. */
void CommitUnitMoveForAttack(void)
{
    ResetCaptureProgressIfMoved();
    gUnknown_030040D8->unk02 = gUnknown_03003100.pos.unk00;
    gUnknown_030040D8->unk03 = gUnknown_03003100.pos.unk02;
    RebuildMapUnitLayers();
}
asm(".global sub_080425B8\n.thumb_set sub_080425B8, CommitUnitMoveForAttack\n");

/* `u8` parameter: the `adds r4, r0, #0; lsls #0x18; lsrs #0x18` prologue is
 * PROMOTE_MODE's copy-then-narrow of a declared-narrow parameter that has to
 * live across the `bl`, not an `int` with a cast. */
void StartRecordedUnitAttack(u8 a)
{
    CommitUnitMoveForAttack();
    StartUnitAttack(a, 1);
}
asm(".global sub_080425E0\n.thumb_set sub_080425E0, StartRecordedUnitAttack\n");

/* StartRecordedUnitAttack's twin, differing only in the constant. */
void StartUnrecordedUnitAttack(u8 a)
{
    CommitUnitMoveForAttack();
    StartUnitAttack(a, 0);
}
asm(".global sub_080425FC\n.thumb_set sub_080425FC, StartUnrecordedUnitAttack\n");

/* Both parameters are WIDE -- bare `adds r4, r0, #0; adds r5, r1, #0` with no
 * narrowing anywhere, which is the bare-prologue rule. */
void StartRecordedInventionAttack(int a, int b)
{
    CommitUnitMoveForAttack();
    StartAttackOnInventionAt(a, b, 1);
}
asm(".global sub_08042618\n.thumb_set sub_08042618, StartRecordedInventionAttack\n");

/* StartRecordedInventionAttack's twin, differing only in the constant. */
void StartUnrecordedInventionAttack(int a, int b)
{
    CommitUnitMoveForAttack();
    StartAttackOnInventionAt(a, b, 0);
}
asm(".global sub_08042634\n.thumb_set sub_08042634, StartUnrecordedInventionAttack\n");
