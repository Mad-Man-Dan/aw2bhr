#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080499F8.
 * sub_080499F8 @ 0x080499F8
 */

void ShopScreen_DrawSprites(void)
{
    PutOamHi(0x7f, 0, gUnknown_084C30FC, 0x6ceb);

    sub_08043FD8((gUnknown_084C30F8->unk832 + 0x18) & 0x1ff, 0xa0, 0x60ab, 0);

    sub_08049944(gUnknown_084C30F8->unk028, 0xe, 4, gUnknown_084C30F8->unk835, 0x3000);

    if (gUnknown_084C30F8->unk835 != 0)
    {
        gUnknown_084C30F8->unk835 = 0;
        BG_EnableSyncBG0();
    }

    DrawOamObject(0x51, 4, 0x20, 0, 0);

    PutSprite(1, 0x00, 0x1c, gUnknown_0848B6C6, 0x8c9b);
    PutSprite(1, 0x20, 0x1c, gUnknown_0848B6C6, 0x8c9b);
    PutSprite(1, 0x40, 0x1c, gUnknown_0848B6C6, 0x8c9b);
    PutSprite(1, 0x60, 0x1c, gUnknown_0848B6C6, 0x8ca3);

    PutSprite(1, 0x00, 0x2c, gUnknown_0848B6BE, 0x8c9f);
    PutSprite(1, 0x20, 0x2c, gUnknown_0848B6BE, 0x8c9f);
    PutSprite(1, 0x40, 0x2c, gUnknown_0848B6BE, 0x8c9f);
    PutSprite(1, 0x60, 0x2c, gUnknown_0848B6BE, 0x8ca7);
}
asm(".global sub_080499F8\n.thumb_set sub_080499F8, ShopScreen_DrawSprites\n");
