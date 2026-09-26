#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F114.
 * sub_0801F114 @ 0x0801F114
 */

void sub_0801F114(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 32; j++)
            gUnknown_0200F920[i].unk08[j].unk00 |= 0xFFFF;
    }
}
