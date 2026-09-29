#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807FE90.
 * sub_0807FE90 @ 0x0807FE90
 */

#include "hardware.h"

void CoPowerScreenBlend_Init(void)
{
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = gUnknown_03001FFC;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 1;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x1E00;
    gUnknown_030030E0.bits.target2_enable_bd = 0;
    SetDispEnable(1, 1, 1, 1, 1);
    RegisterDataMove(gUnknown_080A29A4,
                 (void *)(0x06005600 + gUnknown_03002B6C.bits.chr_block * 0x4000),
                 0x800);
    sub_08012B70(gBG0TilemapBuffer, gUnknown_080A31A4, 0, 0, 0x82B0);
    ApplyPalettes(gUnknown_080A36C8, 8, 1);
    BG_EnableSyncBG0();
}
asm(".global sub_0807FE90\n.thumb_set sub_0807FE90, CoPowerScreenBlend_Init\n");
