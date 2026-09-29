#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D230.
 * sub_0802D230 @ 0x0802D230, sub_0802D2A0 @ 0x0802D2A0
 */

#include "proc.h"

void sub_0802D230(void)
{
    int v;

    LockUnitSelection();
    CloseTopMenu();

    if (((struct Unit *)gUnknown_030040D8)->hp != 0)
        v = (Div(((struct Unit *)gUnknown_030040D8)->hp - 1, 10) + 1)
            * gUnknown_085D5ABC[gUnknown_030040D8->unk00].cost / 2;
    else
        v = 0;

    AddPlayerFunds(gUnknown_030033EC, v);
    CommitUnitMoveBare();
    DestroyUnitAndCargo(gUnknown_03003F38);
    RebuildMapUnitLayers();
}

void UnitMenu_Fire(int a1, int a2, u8 a3)
{
    if (!(a3 & 2))
    {
        FillMovementMap(0xff);
        FillUnitTargetRange(gUnknown_03003100.spos.unk00, gUnknown_03003100.spos.unk02,
            (struct Unit *)gUnknown_030040D8);
        Proc_Start(gUnknown_0849A9F8, PROC_TREE_3);
        CloseTopMenu();
        SetMapLayersTargetBlend();
        IncrementMapLock();
    }
}
asm(".global sub_0802D2A0\n.thumb_set sub_0802D2A0, UnitMenu_Fire\n");
