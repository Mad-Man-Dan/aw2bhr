#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080800B0.
 * sub_080800B0 @ 0x080800B0
 */

#include "hardware.h"
struct Unk080800B0
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void SuperCoPowerScene_InitBackground(struct Unk080800B0 *proc)
{
    gUnknown_030030A4.bits.win0_enable_blend = 0;
    gUnknown_03002B6C.bits.tm_block = 0xE;
    gUnknown_030030B4.bits.tm_block = 0x1A;
    gUnknown_03002B6C.bits.chr_block = 0;
    gUnknown_030030B4.bits.chr_block = 0;
    gUnknown_03002B6C.bits.size = 1;
    gUnknown_030030B4.bits.size = 1;
    Decompress(gUnknown_08235558,
               (void *)(0x06000020 + gUnknown_03002B6C.bits.chr_block * 0x4000));
    Decompress(gUnknown_082352FC, gUnknown_0200FC50);
    AddToHalfwords((u16 *)gUnknown_0200FC50, 0x800, 0x9001);
    ApplyPalettes(gUnknown_08235D10, 9, 1);
    QueueVBlankCallback((void *)SuperCoPowerScene_UploadBgTilemaps);
    gUnknown_03001FF8 = 0x100;
    gUnknown_03001418 = 0xFFF0;
    gUnknown_030030A0 = 0xFF00;
    gUnknown_03001400 = 0xFFF0;
    proc->unk4c = 0;
}
asm(".global sub_080800B0\n.thumb_set sub_080800B0, SuperCoPowerScene_InitBackground\n");
