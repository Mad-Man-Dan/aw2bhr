#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038B84.
 * sub_08038B84 @ 0x08038B84
 */

/* No local for the unk45 index: binding it makes its own statement and moves
 * the gUnknown_0849D5F8 deref AHEAD of the first argument's shift, which is not
 * the ROM's order. Written inline three times it CSEs into r5 and the argument
 * expressions stay in source order. See the unk20/unk2c/unk38 split note in
 * unknown-globals.h for why the three tables are separate members. */
void FillMovementMapFromMovePathEnd(void)
{
    GenerateMovementMapForArmy((gUnknown_03003F38 >> 6) + 1,
                 gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45],
                 gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45],
                 gUnknown_030040D8->unk00,
                 gUnknown_0849D5F8->unk38[gUnknown_0849D5F8->unk45],
                 1);
}
asm(".global sub_08038B84\n.thumb_set sub_08038B84, FillMovementMapFromMovePathEnd\n");
