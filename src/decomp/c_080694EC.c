#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080694EC.
 * sub_080694EC @ 0x080694EC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk694ECProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_IDLE_080694ED(struct Unk694ECProc *proc)
{
    int i;

    switch (0xbe - proc->unk2c)
    {
    case 2:
        SetDispEnable(1, 1, 1, 0, 1);
        Decompress(gUnknown_081837A0, (void *)0x06008000);
        break;

    case 0xb:
        StartIntroSlidePanel(2, 1, 0x1e, proc);
        break;

    case 0x1e:
        TriggerIntroBgAffineTween();
        break;

    case 0x2a:
        for (i = 1; i < 16; i++)
            gPal[i] = 0x7fff;
        EnablePaletteSync();
        EndIntroBgScroll();
        break;

    case 0x2e:
        ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
        StartIntroSlideSprite(2, 0, 0xc, proc);
        StartIntroBgAffineTween(2, 1, 0x120, 0x80, 0, -0x4000, 0x100, 0xc0, 0xc, proc);
        break;

    case 0x5c:
        for (i = 1; i < 16; i++)
            gPal[i] = 0x7fff;
        EnablePaletteSync();
        break;

    case 0x5e:
        SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_03002B6C, 2);
        SetBgCntTilemapBlock((struct Unk8012C30 *)&gUnknown_03002B6C, 0x0600D000);
        Decompress(gUnknown_08183CA8, (void *)0x06004800);
        Decompress(gUnknown_08184A74, gBG0TilemapBuffer);
        for (i = 0; i < 0x400; i++)
            gBG0TilemapBuffer[i] += 0x140;
        BG_EnableSyncBG0();
        break;

    case 0x60:
        ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
        SetIntroSlidePanelExitFrames(0x32);
        TriggerIntroBgAffineTween();
        StartIntroBgBounceIn(proc);
        EndIntroSlideSprite();
        break;
    }

    if (proc->unk2c != 0)
        proc->unk2c--;
    else
        Proc_Break(proc);
}

asm(".global sub_080694EC\n.thumb_set sub_080694EC, IntroT3_IDLE_080694ED\n");
