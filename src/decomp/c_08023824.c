#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023824.
 * sub_08023824 @ 0x08023824
 */

void sub_08023824(void)
{
    if (gUnknown_03004090.unk00 < gUnknown_030032C4.unk00)
        gUnknown_03004090.unk00++;
    if (gUnknown_03004090.unk00 > gUnknown_030032C4.unk00)
        gUnknown_03004090.unk00--;
    if (gUnknown_03004090.unk02 < gUnknown_030032C4.unk02)
        gUnknown_03004090.unk02++;
    if (gUnknown_03004090.unk02 > gUnknown_030032C4.unk02)
        gUnknown_03004090.unk02--;
}
