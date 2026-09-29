#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032104.
 * sub_08032104 @ 0x08032104
 */

void sub_08032104(void)
{
    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 15, 2);
    gUnknown_0300410C = gUnknown_030040CC;
}
