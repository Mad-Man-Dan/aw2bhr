#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B744.
 * sub_0805B744 @ 0x0805B744
 */

void AiFinishLandingPlan(void)
{
    u16 pos[2];

    GenerateUnitMovementMap(gUnknown_030040D8);
    AiListLandingCells();
    if (AiPickUnloadCell(pos) == 1)
        AiUnloadCargoAt(pos);
    AiMoveTowardLandingCell();
}
asm(".global sub_0805B744\n.thumb_set sub_0805B744, AiFinishLandingPlan\n");
