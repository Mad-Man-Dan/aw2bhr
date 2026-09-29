#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802B750.
 * sub_0802B750 @ 0x0802B750
 */

void LatchCursorInfoPanelCell(void)
{
    gUnknown_03003130.unk10 = gUnknown_030033E4.unk00;
    gUnknown_03003130.unk11 = gUnknown_030033E4.unk02;
}
asm(".global sub_0802B750\n.thumb_set sub_0802B750, LatchCursorInfoPanelCell\n");
