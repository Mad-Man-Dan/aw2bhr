#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802428C.
 * sub_0802428C @ 0x0802428C
 */

void RestoreMapCursorPosition(void)
{
    gUnknown_030033E4.unk00 = gUnknown_030040A4.unk00;
    gUnknown_030033E4.unk02 = gUnknown_030040A4.unk02;
    gUnknown_030033E0.unk00 = gUnknown_030040A4.unk00 << 4;
    gUnknown_030033E0.unk02 = gUnknown_030040A4.unk02 << 4;
}
asm(".global sub_0802428C\n.thumb_set sub_0802428C, RestoreMapCursorPosition\n");
