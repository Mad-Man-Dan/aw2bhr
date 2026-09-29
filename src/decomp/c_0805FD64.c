#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FD64.
 * sub_0805FD64 @ 0x0805FD64
 */

/* Probe: is gUnknown_030045D4 VOLATILE? That is the only mechanism left that
 * stops combine folding `sign_extend (mem:HI)` into `ldrsh` -- combine will not
 * touch a volatile MEM, so the read stays an `ldrh` and the narrowing stays a
 * `lsls #0x10; asrs #0x10` pair, which is exactly the ROM's shape.
 */

void AiExecuteActionStep(void)
{
    gUnknown_03004774 = 0;

    switch ((s16)*(volatile u16 *)&gUnknown_030045D4)
    {
    case 0:
        AiExecutorBegin();
        break;
    case 1:
        AiExecutorStartMoveSlide();
        break;
    case 2:
        AiExecutorDispatchAction();
        break;
    case 3:
        AiExecutorStartUnitAttack();
        break;
    case 4:
        AiExecutorStartStructureAttack();
        break;
    case 5:
        AiExecutorLaunchMissile();
        break;
    case 6:
        AiExecutorFinishAfterLaunch();
        break;
    case 7:
        AiExecutorDwellOnTargetUnit();
        break;
    case 8:
        AiExecutorDwellOnTargetCell();
        break;
    case 9:
        AiExecutorDwellOnMissileTarget();
        break;
    case 10:
        AiExecutorFinishAfterDrop();
        break;
    case 11:
        AiExecutorFinishAfterBuy();
        break;
    }
}
asm(".global sub_0805FD64\n.thumb_set sub_0805FD64, AiExecuteActionStep\n");
