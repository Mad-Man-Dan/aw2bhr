#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800057C.
 * sub_0800057C @ 0x0800057C
 */

/*
 * DesignRoomRunMode -- run one frame of the handler for the current map mode.
 *
 * gActiveMap->mode picks the handler; modes 4, 8 and 9 do nothing.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - `case 9:` must stay although its body is empty. The compiler sizes the
 *     jump table from the largest case label, so without it the table has
 *     eight entries instead of ten and the bounds check changes with it.
 */

void DesignRoomRunMode(void)
{
    switch (gActiveMap->mode)
    {
    case 0:
        DesignRoomMode_Start();
        break;
    case 1:
        DesignRoomMode_Paint();
        break;
    case 2:
        DesignRoomMode_Ring();
        break;
    case 3:
        DesignRoomMode_Menu();
        break;
    case 5:
        sub_08000694();
        break;
    case 6:
        sub_08000650();
        break;
    case 7:
        sub_08000664();
        break;
    case 9:
        break;
    }
}
asm(".global sub_0800057C\n.thumb_set sub_0800057C, DesignRoomRunMode\n");
