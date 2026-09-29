#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024274.
 * sub_08024274 @ 0x08024274
 */

void SaveMapCursorPosition(void)
{
    gUnknown_030040A4.unk00 = gUnknown_030033E4.unk00;
    gUnknown_030040A4.unk02 = gUnknown_030033E4.unk02;
}
asm(".global sub_08024274\n.thumb_set sub_08024274, SaveMapCursorPosition\n");
