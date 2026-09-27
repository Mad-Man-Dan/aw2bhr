#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DED8.
 * sub_0801DED8 @ 0x0801DED8, sub_0801DF20 @ 0x0801DF20
 */

void sub_0801DED8(void)
{
    int i;

    for (i = 0; i < gUnknown_03003034; i++)
    {
        if (gUnknown_0200E438[i].unk08)
        {
            RunSimpleSpriteScript(i, 0);
            sub_0801DB04(i);
        }
    }
}

void sub_0801DF20(void)
{
    int i;

    for (i = 0; i < gUnknown_03003034; i++)
        if (gUnknown_0200E438[i].unk08)
            RunSimpleSpriteScript(i, 1);
}
