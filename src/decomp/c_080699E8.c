#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080699E8.
 * sub_080699E8 @ 0x080699E8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk699E8Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_IDLE_080699E9(struct Unk699E8Proc *proc)
{
    switch (0xaa - proc->unk2c)
    {
    case 6:
        SetDispEnable(0, 1, 1, 1, 1);
        StartIntroParallaxScroll(proc);
        break;
    case 0x1f:
        StartIntroCoSlide(0, 0xa, 0x78, 0x78, 0xa, 0, 1, 1, proc);
        break;
    case 0x20:
        StartIntroBlendFade(0, 0xa, 0x28, proc);
        break;
    case 0x2c:
        SetDispEnable(1, 1, 1, 1, 1);
        PauseIntroParallaxScroll();
        break;
    case 0x4a:
        SetDispEnable(0, 1, 1, 1, 1);
    case 0x90:
        ResumeIntroParallaxScroll();
        break;
    case 0x68:
        SetDispEnable(1, 1, 1, 1, 0);
        PauseIntroParallaxScroll();
        break;
    case 0x74:
        SetDispEnable(0, 0, 1, 0, 0);
        SetupIntroBg2Screen(1);
        StartIntroBgZoom(0x100, 0x200, 0, proc);
        break;
    case 0x9a:
        SetupIntroBg2Screen(0);
        SetIntroBgZoomEnabled(1);
        break;
    case 0xa4:
        Proc_EndEach(gUnknown_08581108);
        EndIntroParallaxScroll();
        break;
    }

    if (proc->unk2c != 0)
        proc->unk2c--;
    else
        Proc_Break(proc);
}

asm(".global sub_080699E8\n.thumb_set sub_080699E8, IntroT3_IDLE_080699E9\n");
