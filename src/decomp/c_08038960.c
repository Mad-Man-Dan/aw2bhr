#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038960.
 * sub_08038960 @ 0x08038960, sub_080389D8 @ 0x080389D8, sub_08038AD8 @ 0x08038AD8
 */

/* Linear search of the move stack for the (x, y) step `a`,`b`, returning its
 * slot index or -1. `i` is s8, not int: the ROM re-narrows the counter with
 * `lsrs #0x18` at the increment and sign-extends it again at each use. */
int FindMovePathStep(s8 a, s8 b)
{
    s8 i;

    for (i = 0; i <= gUnknown_0849D5F8->unk45; i++)
    {
        if (gUnknown_0849D5F8->unk20[i] == a && gUnknown_0849D5F8->unk2c[i] == b)
            return i;
    }

    return -1;
}
asm(".global sub_08038960\n.thumb_set sub_08038960, FindMovePathStep\n");

/* Replays the direction string EncodeMovePathDirections wrote into gUnknown_03003110,
 * pushing one step per byte onto the move stack via PushMovePathStep, and stops on
 * the 4 terminator.
 *
 * The jump table is 11 entries wide with a `+1` bias, so the case values run
 * -1..9: agbcc's expand_case subtracts the lowest case, which is -1 here, and
 * the table extends to 9 because that is the highest case in the source. Cases
 * 5..9 do nothing, so their entries and the default both land on the loop top.
 *
 * `case 3` is written BEFORE `case 2`: bodies are laid out in source order, and
 * the ROM puts the y-1 block at 0x08038A80 ahead of the y+1 block at
 * 0x08038AA4 while the jump table still lists them by case value. Writing them
 * in numeric order swaps the two blocks and moves the tail-merge point.
 *
 * gUnknown_08090F0C is this unit's own -fforce-addr pool word holding
 * &gUnknown_03003110; EncodeMovePathDirections has a second, private copy at 0x08090F10. */
void RebuildMovePathFromDirections(void)
{
    s8 i;

    i = 0;

    for (;;)
    {
        switch (gUnknown_03003110[i++])
        {
        case -1:
            return;
        case 0:
            PushMovePathStep(gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45] - 1,
                         gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45]);
            break;
        case 1:
            PushMovePathStep(gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45] + 1,
                         gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45]);
            break;
        case 3:
            PushMovePathStep(gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45],
                         gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45] - 1);
            break;
        case 2:
            PushMovePathStep(gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45],
                         gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45] + 1);
            break;
        case 4:
            return;
        case 9:
            break;
        }
    }
}
asm(".global sub_080389D8\n.thumb_set sub_080389D8, RebuildMovePathFromDirections\n");

/* Encodes the move stack as a direction string in gUnknown_03003110, one byte
 * per step, terminated by 4. The codes are the ones RebuildMovePathFromDirections replays:
 * 0 = x-1, 1 = x+1, 2 = y+1, 3 = y-1. The trailing store uses the loop
 * counter after the loop, so `i` must be the same s8 the loop leaves behind.
 *
 * gUnknown_08090F10 is agbcc's own -fforce-addr pool word holding
 * &gUnknown_03003110 -- the honest spelling emits it, exactly as the wave-41
 * gUnknown_08090940 note in unknown-globals.h records. RebuildMovePathFromDirections has its
 * own private copy of the same address at 0x08090F0C. */
void EncodeMovePathDirections(void)
{
    s8 i;

    for (i = 1; i <= gUnknown_0849D5F8->unk45; i++)
    {
        if (gUnknown_0849D5F8->unk20[i] < gUnknown_0849D5F8->unk20[i - 1])
            gUnknown_03003110[i - 1] = 0;
        else if (gUnknown_0849D5F8->unk20[i] > gUnknown_0849D5F8->unk20[i - 1])
            gUnknown_03003110[i - 1] = 1;
        else if (gUnknown_0849D5F8->unk2c[i] < gUnknown_0849D5F8->unk2c[i - 1])
            gUnknown_03003110[i - 1] = 3;
        else
            gUnknown_03003110[i - 1] = 2;
    }

    gUnknown_03003110[i - 1] = 4;
}
asm(".global sub_08038AD8\n.thumb_set sub_08038AD8, EncodeMovePathDirections\n");
