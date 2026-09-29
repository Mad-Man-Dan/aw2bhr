#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A99C.
 * sub_0807A99C @ 0x0807A99C
 */

#include "hardware.h"

void LoadBg3MapBackdrop(s32 a1, u8 a2)
{
    if (a1 != 0)
        a1--;
    else
        a1 = DivRem(gGameClock, 0x12);

    Decompress(gUnknown_08615E4C[a1].unk00,
               (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06000000));
    Decompress(gUnknown_08615E4C[a1].unk04,
               (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06001B00));
    Decompress(gUnknown_08615E4C[a1].unk08,
               (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06003600));
    Decompress(gUnknown_08615E4C[a1].unk0c, gBG3TilemapBuffer);

    BG_EnableSyncBG3();

    if (a1 == 7)
        ApplyPaletteExt(gUnknown_08615E4C[a1].unk10, a2 * 0x20, 0xE0);
    else
        ApplyPaletteExt(gUnknown_08615E4C[a1].unk10, a2 * 0x20, 0xC0);
}
asm(".global sub_0807A99C\n.thumb_set sub_0807A99C, LoadBg3MapBackdrop\n");
