#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069D3C.
 * sub_08069D3C @ 0x08069D3C, sub_08069DE8 @ 0x08069DE8
 */

#include "hardware.h"
#include "proc.h"
/* Sets up the two-sided slide IntroCoReveal_Loop drives: unk38 picks the side and
 * every constant mirrors across the two arms. Same proc as
 * src/decomp/c_08069EAC.c -- unk38 is the discriminator there too.
 *
 * unk34 is an `int`, NOT a u16, even though StartIntroCoSlide reads it with `ldrh`:
 * StartIntroCoNameBanner reads the SAME offset with a full `ldr` two instructions later.
 * The `ldrh` is StartIntroCoSlide's u16 parameter narrowing a memory operand in
 * place, which agbcc does without a separate shift pair.
 *
 * `proc->unk2c = proc->unk2e = 0;` is one chained assignment, which is why
 * +0x2e is stored before +0x2c on both arms. */
struct Unk69DE8Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 filler_32[0x02];
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
};

/* Window setup for the two-sided slide IntroCoReveal_Loop drives; the argument picks
 * which side and indexes a flat u16 table for the two window bounds.
 *
 * gUnknown_030030A4 is written through its win0_* group, NOT win1_* -- the ROM
 * reads and writes BYTE 0 of it (`ldrb r1, [r4]`), and win1_* lives in byte 1.
 * src/decomp/c_0806B9CC.c uses win1_* on this same shadow because that function
 * touches the other half; the shadow carries both groups.
 *
 * The 0x20 constant lives in sb for the whole body because it is needed twice,
 * once for SetWinEnable's win0 and once for gUnknown_030030DC's
 * win0_enable_blend at the very end -- that is what the `mov r6, sb` prologue
 * is holding, not a loop. That final `orr #0x20` re-uses the byte value already
 * in r2 rather than reloading it: the four stores in between are to different
 * globals, so nothing invalidates it. */
void SetIntroSplitWindow(int a1)
{
    SetWinEnable(1, 0, 0);

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 0;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_03002B40 = gUnknown_08581478[a1 * 2];
    gUnknown_03002EFC = 0;
    gUnknown_03002B4C = gUnknown_08581478[a1 * 2 + 1];
    gUnknown_03002B44 = 0xa0;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
}
asm(".global sub_08069D3C\n.thumb_set sub_08069D3C, SetIntroSplitWindow\n");

void IntroCoReveal_Init(struct Unk69DE8Proc *proc)
{
    Proc_EndEach(gUnknown_08581108);

    if (proc->unk38 == 0)
    {
        StartIntroCoSlide(proc->unk38, proc->unk34, 0x3c, 0x50, proc->unk3c, 1, proc->unk38, 1, proc);
        StartIntroBlendFade(0, 0x10, proc->unk3c - 0x18, proc);
        StartIntroCoNameBanner(proc->unk38, proc->unk34, 0x280, 3, proc);
        proc->unk2a = 0xe8;
        proc->unk2c = proc->unk2e = 0;
        proc->unk30 = 0xffc8;
        SetIntroSplitWindow(1);
    }
    else
    {
        StartIntroCoSlide(proc->unk38, proc->unk34, 0xb4, 0xa0, proc->unk3c, 1, 1, 1, proc);
        StartIntroBlendFade(0, 0x10, proc->unk3c - 0x18, proc);
        StartIntroCoNameBanner(proc->unk38, proc->unk34, 0x280, 3, proc);
        proc->unk2a = 0xffd0;
        proc->unk2c = proc->unk2e = 0;
        proc->unk30 = 0x98;
        SetIntroSplitWindow(0);
    }
}
asm(".global sub_08069DE8\n.thumb_set sub_08069DE8, IntroCoReveal_Init\n");
