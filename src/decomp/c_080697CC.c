#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080697CC.
 * sub_080697CC @ 0x080697CC, sub_08069864 @ 0x08069864, sub_08069924 @ 0x08069924
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* A screen-setup proc entry in the sub_08068AC4 family: take a ticket, rebuild
 * the display state, load graphics through LoadIntroScreenGraphicsWithBg1 and reset all four
 * scroll channels.
 *
 * The five bg/obj enables are ONE ldrb/strb pair around five `orr`s because
 * they are consecutive read-modify-writes of the same byte -- CSE drops the
 * reloads and dead-store elimination drops the intermediate stores. That is
 * SetDispEnable, not a hand-merged mask.
 *
 * The four priority stores are `priority = 0/1/2/3` in gUnknown_03002B6C,
 * 03001FE8, 0300251C, 030030B4 order; -4 is materialised once and reused. The
 * last one is a bare `orr #3` with no `and` because `(x & ~3) | 3` folds. */
struct Unk69864Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

/* Graphics loader; LoadIntroScreenGraphics is its near-twin, differing only in the first
 * Decompress destination (0x06000000 here, 0x06008000 there), one extra blob
 * into *gBG1TilemapBuffer, and the extra BG_EnableSyncBG1 call.
 *
 * The zero word CpuFastSet fills from is a STACK local, which is what the
 * `sub sp, #4` and `mov r0, sp` are for. gUnknown_08580E60's address stays in
 * r4 across the whole body because it is dereferenced three times. */
void LoadIntroScreenGraphicsWithBg1(void)
{
    int zero;

    zero = 0;
    CpuFastSet(&zero, gUnknown_08580E60, 0x01000400);
    Decompress(gUnknown_08184FF4, (void *)0x06000000);
    Decompress(gUnknown_08185F0C, (void *)0x0600C000);
    ApplyPaletteExt(gUnknown_081866D8, 0xc0, 0x20);
    Decompress(gUnknown_0818616C, gBG1TilemapBuffer);
    Decompress(gUnknown_0818633C, gBG2TilemapBuffer);
    Decompress(gUnknown_08186460, gUnknown_08580E60);
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    RegisterDataMove(gUnknown_08580E60, (void *)0x0600F000, 0x1000);
}
asm(".global sub_080697CC\n.thumb_set sub_080697CC, LoadIntroScreenGraphicsWithBg1\n");

void IntroT3_08069865(struct Unk69864Proc *proc)
{
    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    ApplyBgControlTable(gUnknown_08581438);
    gDispIo.disp_ct.mode = 0;
    SetDispEnable(1, 1, 1, 1, 1);
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 3;
    SetDefaultColorEffects();
    LoadIntroScreenGraphicsWithBg1();
    SetBgScrollShadow(0, 0, 0);
    SetBgScrollShadow(1, 0, 0);
    SetBgScrollShadow(2, 0, 0);
    SetBgScrollShadow(3, 0, 0);
}

/* The mode-1 counterpart of IntroT3_08069865's setup: reorder the four BG
 * priorities, hand gUnknown_030030B4 to SetBgCntScreenSize, clear the tile buffer and
 * load graphics. The one blob is conditional on the argument.
 *
 * The argument is `u8` -- the `lsl #0x18; lsr #0x18` at entry is PROMOTE_MODE
 * on a sub-word parameter, and the value survives in r6 to be truth-tested
 * later, so it is a real narrow parameter and not a cast at a use.
 *
 * `ldrb [r,#1]; orr #0x20; strb` is bit 13 of the halfword, which is BgCnt's
 * `wrap`. Wave 23's note in hardware.h said nothing had yet reached that field;
 * this is the first site that does. */
void SetupIntroBg2Screen(u8 a1)
{
    int zero;

    gDispIo.disp_ct.mode = 1;
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    ResetBgAffineToScreenCentre();
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_030030B4, 1);
    zero = 0;
    CpuFastSet(&zero, gBG2TilemapBuffer, 0x01000200);
    gUnknown_030030B4.bits.wrap = 1;
    ApplyPaletteExt(gUnknown_0817DA18, 0x20, 0x20);
    if (a1 != 0)
        Decompress(gUnknown_0817DA38, (void *)0x06008000);
    Decompress(gUnknown_0817E208, gBG2TilemapBuffer);
    BG_EnableSyncBG2();
}
asm(".global sub_08069924\n.thumb_set sub_08069924, SetupIntroBg2Screen\n");

asm(".global sub_08069864\n.thumb_set sub_08069864, IntroT3_08069865\n");
