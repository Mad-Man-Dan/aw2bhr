#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080339B0.
 * sub_080339B0 @ 0x080339B0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* Only +0x2c is touched on the caller's proc, and it holds the child started
 * from gUnknown_0849BC50. */
struct Unk339B0Child;
struct Unk339B0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ struct Unk339B0Child *unk2c;
};
struct Unk339B0Child
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x36);
    /* 36 */ u8 unk36;
    /* 37 */ u8 unk37;
    /* 38 */ STRUCT_PAD(0x38, 0x40);
    /* 40 */ int unk40;
    /* 44 */ int unk44;
    /* 48 */ int unk48;
    /* 4c */ int unk4c;
};

void LinkC2_080339B1(struct Unk339B0Proc *proc)
{
    struct Unk339B0Child *child;

    SetupBackgrounds(gUnknown_0849D16C);
    InitTextTileCache(0x70);
    LinkScreenSetupMessageWindow();
    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();

    ApplyPalettes(gUnknown_081D3E88, 17, 3);
    Decompress(gUnknown_081D8A54, (void *)0x06015780);
    CpuCopyAuto(gUnknown_0812B49C, (void *)0x06010200, 0x180);
    CpuCopyAuto(gUnknown_0812B61C, (void *)0x06010380, 0xe0);
    CpuCopyAuto(gUnknown_0812B6FC, (void *)0x06010460, 0x120);
    ApplyPalette(gUnknown_0809165C, 21);

    sub_0801F114();
    InitTilePool(2, (void *)0x06010000, 0, 0x16);
    LoadTilePoolGraphic(0x50);

    ApplyPalette(gUnknown_081320AC, 3);
    ApplyPalette(gUnknown_081D92B8, 0);

    Decompress(gUnknown_0823A3D4,
               BG_CHAR_ADDR(((union BgCntBuf *)&gUnknown_0300251C)->bits.chr_block));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    BG_EnableSyncBG3();

    StartHeaderBanner(gUnknown_085802F0, gUnknown_02010C50, 0xec, 0xf, 0, 0, (int)proc);

    Proc_Start(gUnknown_0849BB80, proc);
    Proc_Start(ProcScr_PutFace, proc);

    child = Proc_Start(gUnknown_0849BC50, proc);
    child->unk4c = 0;
    child->unk48 = -1;
    child->unk44 = -1;
    child->unk40 = -1;
    child->unk36 = 1;
    child->unk37 = 1;
    proc->unk2c = child;

    LinkScreenSetMessage(0, gUnknown_0849BC3E[child->unk36], 0x40);
    sub_08085AF4();
    sub_0803D48C();
}

asm(".global sub_080339B0\n.thumb_set sub_080339B0, LinkC2_080339B1\n");
