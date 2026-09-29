#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FFA0.
 * sub_0805FFA0 @ 0x0805FFA0
 */

void AiExecutorDispatchAction(void)
{
    if (gUnknown_030046C0.unk00 != 1
     && gUnknown_030046C0.unk00 != 0xe
     && gUnknown_030046C0.unk00 != 0xd
     && gUnknown_030046C0.unk00 != 0xf
     && gUnknown_030046C0.unk00 != 0x10
     && gUnknown_030046C0.unk00 != 0x11
     && gUnknown_030046C0.unk00 != 0x12
     && gUnknown_030046C0.unk00 != 0x13)
    {
        gUnknown_030040D8->unk01 |= 1;
        gUnknown_030040D8->unk02 = gUnknown_03003100.pos.unk00;
        gUnknown_030040D8->unk03 = gUnknown_03003100.pos.unk02;
        RebuildMapUnitLayers();
    }

    switch (gUnknown_030046C0.unk00)
    {
    case 1:
        AiExecuteBuildUnit();
        break;
    case 0xe:
        AiExecuteBuildUnitWithRole();
        break;
    case 0xd:
        AiExecuteTurnEnd();
        break;
    case 0xf:
        AiExecuteCoPower();
        break;
    case 0x10:
        AiExecuteSuperCoPower();
        break;
    case 3:
        ApplyCaptureProgress();
    case 2:
    _redraw:
        CommitUnitMove();
        break;
    case 4:
        AiExecutorCheckTargetUnitVisible();
        return;
    case 5:
        AiExecutorCheckTargetCellVisible();
        return;
    case 7:
        LoadUnitIntoTransport();
        goto _redraw;
    case 8:
        AiExecutorStartCargoDrop(1);
        AiExecutorStartCargoDrop(0);
        gUnknown_030045D4 = 0xa;
        return;
    case 0xa:
        sub_08042998();
        goto _redraw;
    case 6:
        sub_08042B84();
        break;
    case 0xb:
        DiveSelectedUnit();
        goto _redraw;
    case 0xc:
        SurfaceSelectedUnit();
        goto _redraw;
    case 0x12:
        AiExecuteDestroyUnit();
        break;
    case 0x13:
        ApplyYieldCommand();
        break;
    case 0x14:
        AiExecutorCheckMissileTargetVisible();
        return;
    }

    gUnknown_03004780 = 2;
    gUnknown_030045D4 = 0;
}
asm(".global sub_0805FFA0\n.thumb_set sub_0805FFA0, AiExecutorDispatchAction\n");
