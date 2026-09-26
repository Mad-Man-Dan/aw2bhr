#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B768.
 * sub_0801B768 @ 0x0801B768
 */

void sub_0801B768(int a)
{
    gUnknown_03002B80.unk358 = a + 1;
    gUnknown_03002B80.unk00 = 1;
}
