#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B708.
 * sub_0806B708 @ 0x0806B708
 */

#include "hardware.h"
#include "proc.h"
/* A fourth member of the screen-setup family (sub_0806A054, sub_08068AC4,
 * sub_080688E4). Same skeleton; the two things worth reading off it:
 *
 * The CpuFastSet destination is computed from the BG shadow's own chr_block
 * field -- `ldr` of the union, `lsl #28; lsr #30`, `lsl #14` -- which is the
 * same `gUnknown_03001FE8.bits.chr_block * 0x4000` idiom already in
 * c_08013C54.c, not an opaque constant. The `ldr` on a two-byte union is
 * normal for the bitfield view; see the width note on union BgCntBuf.
 *
 * The `u32 fill` local is what `sub sp, #4` buys, exactly as in sub_080688E4:
 * CpuFastSet's fill form takes the address of a word. */
struct Unk0806B708
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 filler_2e[0x02];
    /* 0x30 */ u32 unk30;
};

void CreditsMissionList_Init(struct Unk0806B708 *proc)
{
    u32 fill;

    ResetWindowShadows();
    SetDefaultColorEffects();
    SetupBackgrounds(gUnknown_0849D16C);
    gDispIo.disp_ct.obj_mapping = 1;
    gPal[1] = 0;
    ApplyPalettes(gUnknown_0822DE80, 0x10, 1);
    fill = 0x11111111;
    CpuFastSet(&fill,
               (void *)(0x06005000 + gUnknown_03001FE8.bits.chr_block * 0x4000),
               0x01000008);
    FillHalfwordsUnsigned(gBG1TilemapBuffer, 0x400, 0x280);
    Decompress(gUnknown_081918A4, (void *)0x06016000);
    SetDispEnable(0, 1, 0, 1, 1);
    gUnknown_03001FE8.bits.priority = 0;
    gUnknown_0300251C.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 3;
    BuildCreditsMissionList();
    proc->unk2c = 0;
    proc->unk2a = 0;
    proc->unk30 = 0;
    StartCreditsIllustration(proc);
    BG_EnableSyncBG1();
    Proc_Start(gUnknown_0858193C, proc);
}
asm(".global sub_0806B708\n.thumb_set sub_0806B708, CreditsMissionList_Init\n");
