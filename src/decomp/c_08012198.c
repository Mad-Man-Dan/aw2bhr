#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012198.
 * sub_08012198 @ 0x08012198
 */

void sub_08012198(void)
{
    int i;

    PutHexNumberSprites(8, 0, (s16)gUnknown_03002F30);

    for (i = 0; i <= 3; i++)
    {
        PutHexNumberSprites(0x40, i * 8 + 8, gUnknown_0200B3B4[i].unk00);
        PutHexNumberSprites(0x84, i * 8 + 8, gUnknown_0200B3B4[i].unk04);
        PutHexNumberSprites(0x98, i * 8 + 8,
                     gUnknown_0200B3B4[i].unk08 * (gUnknown_0200B3B4[i].unk0a + 1) * 2);
    }
}
