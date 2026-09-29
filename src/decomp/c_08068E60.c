#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08068E60.
 * sub_08068E60 @ 0x08068E60
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
struct Unk08068E60
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
};

void IntroT3_08068E61(struct Unk08068E60 *proc)
{
    int i;
    u32 zero0;
    u32 zero1;
    u32 zero2;
    u32 zero3;

    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    gDispIo.disp_ct.mode = 1;
    SetDispEnable(1, 1, 1, 0, 1);
    gUnknown_030030B4.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
    ResetBgAffineToScreenCentre();
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_03001FE8, 1);
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_030030B4, 2);
    gUnknown_030030B4.bits.wrap = 0;
    zero0 = 0;
    CpuFastSet(&zero0, (void *)0x0600E000, 0x01000400);
    zero1 = 0;
    CpuFastSet(&zero1, gBG2TilemapBuffer, 0x01000400);
    ApplyPalettes(gUnknown_08183C28, 0, 4);
    ApplyPalette((u16 *)gUnknown_0823BDE0, 0);
    Decompress(gUnknown_081837A0, (void *)0x06008000);
    Decompress(gUnknown_081838EC, gBG2TilemapBuffer);
    Decompress(gUnknown_0823A3D4, (void *)0x06002800);
    Decompress(gUnknown_08239FA4, gBG0TilemapBuffer);

    for (i = 0; i < 0x400; i++)
        gBG0TilemapBuffer[i] += 0x140;

    zero2 = 0;
    CpuFastSet(&zero2, (void *)0x06000000, 0x01000008);
    zero3 = 0;
    CpuFastSet(&zero3, (void *)0x06008000, 0x01000008);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    SetBgScrollShadow(0, 0, 0);
    SetBgScrollShadow(1, 0, 0);
    SetBgScrollShadow(2, 0, 0);
    StartIntroBgScroll(0, 4, 4, proc);
    StartIntroBgAffineTween(1, 1, 0, 0x88, 0x3800, 0, 0xc0, 0x100, 0xe, proc);
}

asm(".global sub_08068E60\n.thumb_set sub_08068E60, IntroT3_08068E61\n");
