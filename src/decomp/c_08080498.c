#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080498.
 * sub_08080498 @ 0x08080498
 */

#include "hardware.h"
#include "proc.h"
struct Unk08080498
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void SuperCoPowerScene_StartNameStage(struct Unk08080498 *proc)
{
    SetupMenuScreenBgs(proc);
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    Decompress(gUnknown_08236294,
               (void *)(0x06000000 + gUnknown_0300251C.bits.chr_block * 0x4000));
    Decompress(gUnknown_08235D30, gBG3TilemapBuffer);
    ApplyPalettes(gUnknown_082391E8, 0, 1);
    BG_EnableSyncBG3();
    SetDispEnable(0, 1, 1, 1, 1);
    RegisterDataMove(gUnknown_080A29A4,
                 (void *)(0x06005600 + gUnknown_03002B6C.bits.chr_block * 0x4000),
                 0x800);
    sub_08012B70(gBG0TilemapBuffer, gUnknown_080A31A4, 0, 0, 0x82B0);
    ApplyPalettes(gUnknown_080A36C8, 8, 1);
    BG_EnableSyncBG0();
    LoadCoFullBodyAndPalette(gUnknown_03005970, 0x1CA, 1);
    Decompress(gUnknown_08239228, gUnknown_0200FC50);
    CpuFastSet(gUnknown_0200FC50, (void *)0x06017740, 8);
    ApplyPalettes(gUnknown_08239DE4, 0x10, 1);
    proc->unk4c = 0;
    Proc_Start(gUnknown_086168BC, proc);
    StartBgWave(0x200, 0x100, 0x180, 0x100, 2, 2);
}
asm(".global sub_08080498\n.thumb_set sub_08080498, SuperCoPowerScene_StartNameStage\n");
