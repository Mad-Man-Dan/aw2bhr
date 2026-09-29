#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080691BC.
 * sub_080691BC @ 0x080691BC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk691BCProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_IDLE_080691BD(struct Unk691BCProc *proc)
{
    int i;

    switch (0xc6 - proc->unk2c)
    {
    case 5:
        StartIntroSlidePanel(1, -1, 0x20, proc);
        break;

    case 0x1a:
        TriggerIntroBgAffineTween();
        break;

    case 0x26:
        for (i = 1; i < 16; i++)
            gPal[i] = 0x7fff;
        EnablePaletteSync();
        ResetIntroBgScroll();
        break;

    case 0x2a:
        ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
        StartIntroSlideSprite(1, 1, 0xc, proc);
        StartIntroBgAffineTween(0, -1, 0, 0x88, 0, 0x4000, 0x100, 0xc0, 0xc, proc);
        break;

    case 0x5c:
        for (i = 1; i < 16; i++)
            gPal[i] = 0x7fff;
        EnablePaletteSync();
        break;

    case 0x60:
        ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
        SetIntroSlidePanelExitFrames(0x28);
        ResumeIntroBgScroll();
        TriggerIntroBgAffineTween();
        EndIntroSlideSprite();
        break;

    case 0x8c:
        SetDispEnable(1, 1, 0, 0, 1);
        ResetBgAffineToScreenCentre();
        SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_030030B4, 1);
        StartBlendRampWhite0To16(0x30, 1, proc);
        break;

    case 0xb9:
        Decompress(gUnknown_0817DA38, (void *)0x06008000);
        break;

    case 0xba:
        Decompress(gUnknown_0817E208, gBG2TilemapBuffer);
        ApplyPaletteExt(gUnknown_0817DA18, 0x20, 0x20);
        BG_EnableSyncBG2();
        break;

    case 0xbe:
        SetDefaultColorEffects();
        SetDispEnable(0, 0, 1, 0, 1);
        EndIntroBgScroll();
        break;
    }

    if (proc->unk2c != 0)
        proc->unk2c--;
    else
        Proc_Break(proc);
}

asm(".global sub_080691BC\n.thumb_set sub_080691BC, IntroT3_IDLE_080691BD\n");
