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
        sub_0802E698();
        break;

    case 4:
        sub_0802E6C0();
        break;

    case 5:
        sub_0802E6F8();
        break;

    case 6:
        sub_0802DFC8();
        break;

    case 7:
        sub_0802E260();
        break;

    case 8:
        sub_0802E278();
        break;
    }
}
asm(".global sub_0802DC2C\n.thumb_set sub_0802DC2C, RunMapCursorState\n");
