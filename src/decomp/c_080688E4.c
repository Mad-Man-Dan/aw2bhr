#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080688E4.
 * sub_080688E4 @ 0x080688E4
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
/* Sibling of sub_08068AC4 and sub_0806A054 -- same screen-setup shape, this one
 * on BG mode 1 with the BG2 tilemap cleared through CpuFastSet.
 *
 * The `u32 zero` local is real and is what `sub sp, #4` buys: CpuFastSet's fill
 * form needs the address of a word, so the constant cannot stay in a register.
 *
 * `gUnknown_030030B4.bits.wrap = 0` is the `movs #0x21; rsbs` at the end -- bit
 * 13 of the halfword is bit 5 of byte 1, and the `ldrb [r4,#1]` / `strb [r4,#1]`
 * pair is the byte view of the same union the four priority stores use. */
struct Unk080688E4
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u32 unk34;
};

void IntroT3_080688E5(struct Unk080688E4 *proc)
{
    u32 zero;

    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    gDispIo.disp_ct.mode = 1;
    SetDispEnable(1, 1, 0, 0, 1);
    gUnknown_030030B4.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    ResetBgAffineToScreenCentre();
    SetBgCntChrBlock((struct Unk8012C30 *)&gUnknown_030030B4, 0x06008000);
    SetBgCntTilemapBlock((struct Unk8012C30 *)&gUnknown_030030B4, 0x0600F000);
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_030030B4, 1);
    zero = 0;
    CpuFastSet(&zero, (void *)0x0600F000, 0x01000100);
    gUnknown_030030B4.bits.wrap = 0;
    Decompress(gUnknown_0817DA38, (void *)0x06008000);
    Decompress(gUnknown_0817E208, gBG2TilemapBuffer);
    BG_EnableSyncBG2();
    StartIntroBgZoom(0xC00, -0x28, 0, proc);
    proc->unk30 = 0;
    proc->unk34 = 0;
}

asm(".global sub_080688E4\n.thumb_set sub_080688E4, IntroT3_080688E5\n");
