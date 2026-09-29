#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D168.
 * sub_0802D168 @ 0x0802D168, sub_0802D1A0 @ 0x0802D1A0, sub_0802D1C0 @ 0x0802D1C0, sub_0802D1F8 @ 0x0802D1F8
 */

/* An if/else and NOT `g = (cond != 0)`: two separate `strb`s split across an
 * unconditional `b`, which is the spelling `return <cmp>` / a stored comparison
 * cannot produce (docs/agbcc-codegen.md -- a stored flag goes through
 * do_store_flag and arrives with no unconditional branch). The else arm reuses
 * the register the guard already loaded the zero into, hence `strb r1`.
 *
 * gUnknown_030033E8 is declared `u8 []` (two bytes are known written); this
 * reads element 0. The two pool words for gUnknown_03000558 are -fforce-addr
 * giving each arm its own copy. */

void UnitMenu_DropFirst(void)
{
    if (gUnknown_030033E8[0] != 0)
        gUnknown_03000558 = 1;
    else
        gUnknown_03000558 = 0;

    StartDropCellPicker(0);
    CloseTopMenu();
    IncrementMapLock();
}
asm(".global sub_0802D168\n.thumb_set sub_0802D168, UnitMenu_DropFirst\n");

/* The unconditional twin of UnitMenu_DropFirst: the flag is forced to 1 and the
 * StartDropCellPicker argument goes with it. The two `movs r0,#1` are not CSEd
 * because the `strb` and the argument are separate values to gcc. */

void UnitMenu_DropSecond(void)
{
    gUnknown_03000558 = 1;

    StartDropCellPicker(1);
    CloseTopMenu();
    IncrementMapLock();
}
asm(".global sub_0802D1A0\n.thumb_set sub_0802D1A0, UnitMenu_DropSecond\n");

/* See src/decomp/c_0802D064.c: same shape with two of the six calls dropped,
 * DiveSelectedUnit in the middle and command id 0xb. */

void UnitMenu_Dive(void)
{
    LockUnitSelection();
    CloseTopMenu();
    DiveSelectedUnit();
    CommitUnitMove();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0xB, gUnknown_03003F38, 0, 0);
}
asm(".global sub_0802D1C0\n.thumb_set sub_0802D1C0, UnitMenu_Dive\n");

/* See src/decomp/c_0802D064.c and c_0802D1C0.c: the SurfaceSelectedUnit twin of
 * UnitMenu_Dive, command id 0xc. */

void UnitMenu_Rise(void)
{
    LockUnitSelection();
    CloseTopMenu();
    SurfaceSelectedUnit();
    CommitUnitMove();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0xC, gUnknown_03003F38, 0, 0);
}
asm(".global sub_0802D1F8\n.thumb_set sub_0802D1F8, UnitMenu_Rise\n");
