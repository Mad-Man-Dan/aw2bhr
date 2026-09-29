#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022AF8.
 * sub_08022AF8 @ 0x08022AF8
 */

void DrawCornerBracketSprites(u8 a, u8 b, u8 c, u8 d)
{
    u8 t;

    t = (u32)gGameClock % 33;

    if (t < 5)
        t = 2;
    else if (t > 8)
    {
        if (t <= 0x1c)
            t = 0;
        else
            t = 1;
    }
    else
        t = 1;

    PutOamHi(a + t, (b + t) | 0x400, gUnknown_08499B6C, 0x1365);
    PutOamHi(a + c - t, (b + t) | 0x400, gUnknown_08499B74, 0x1365);
    PutOamHi(a + t, (d + b - t) | 0x400, gUnknown_08499B7C, 0x1365);
    PutOamHi(a + c - t, (d + b - t) | 0x400, gUnknown_08499B84, 0x1365);
}
asm(".global sub_08022AF8\n.thumb_set sub_08022AF8, DrawCornerBracketSprites\n");
