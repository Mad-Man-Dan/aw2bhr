#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DC2C.
 * sub_0802DC2C @ 0x0802DC2C
 */

void RunMapCursorState(void)
{
    switch (gUnknown_03003334)
    {
    case 0:
        MapCursorIdle();
        break;

    case 1:
        MapCursorState_ChooseDestination();
        break;

    case 2:
        MapCursorState_DeleteUnit();
        break;

    case 3:
        MapCursorState_OpenUnitMenu();
        break;

    case 4:
        MapCursorState_UnitMenuOpen();
        break;

    case 5:
        MapCursorState_Ambushed();
        break;

    case 6:
        MapCursorState_RangeWhileBHeld();
        break;

    case 7:
        sub_0802E260();
        break;

    case 8:
        MapCursorState_UnitsTranslucent();
        break;
    }
}
asm(".global sub_0802DC2C\n.thumb_set sub_0802DC2C, RunMapCursorState\n");
