#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034F1C.
 * sub_08034F1C @ 0x08034F1C
 */

void MapState_ResumeCursorAfterCommand(void)
{
    if (gUnknown_03002F1C != 0)
    {
        PopMenu();
        IncrementMapLock();
        gUnknown_03002F1C = 0;
    }

    gUnknown_030032D8 = 0xd;
}
asm(".global sub_08034F1C\n.thumb_set sub_08034F1C, MapState_ResumeCursorAfterCommand\n");
