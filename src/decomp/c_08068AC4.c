#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08068AC4.
 * sub_08068AC4 @ 0x08068AC4, sub_08068BE4 @ 0x08068BE4
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
/* A 0x08068AC4 screen-setup proc entry, sibling of sub_0806A054 and
 * sub_080688E4: take a ticket, rebuild the display state, load graphics, start
 * the follow-up proc and lift the forced blank.
 *
 * The four BG-priority stores are `priority = 0/1/2/3` in gUnknown_0300251C,
 * 030030B4, 03001FE8, 03002B6C order -- the mask constant -4 is materialised
 * once as `movs #0x11; rsbs; adds #0xd` off the DISPCNT mask above it and then
 * reused, which is the CSEd-constant chain and not four different values.
 *
 * `forced_blank = 0` is a bitfield store even though it compiles to a bare
 * `movs r0, #0x7f; ands` with no `neg`: the field is the top bit of the byte,
 * so its complement mask already fits an 8-bit immediate. */
struct Unk08068AC4
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u32 unk30;
};
#include "proc.h"
/* One frame of a 0xcc-tick countdown cutscene, driven off `0xcc - unk2c`. The
 * switch is on `t >> 1` -- an arithmetic shift, not `/ 2`, which would have
 * carried the signed rounding correction -- so only even ticks reach a case.
 * The four graphics cases differ in blob and destination cursor only; case 102
 * is the end, which turns the blend on hard and breaks the proc. The tail runs
 * on every tick including the default, so it sits after the switch rather than
 * in a default arm.
 *
 * The `(u16)` cast on the last argument is NOT decoration: the ROM narrows it
 * with `lsls #16; lsrs #16` and the shared prototype for SetBgScrollShadow declares
 * that parameter `u32`. Its third parameter is really `u16`; the cast is here
 * so this file does not have to retype another agent's declaration. */

/* One frame of a 0xcc-tick countdown cutscene, driven off `0xcc - unk2c`. The
 * switch is on `t >> 1` -- an arithmetic shift, not `/ 2`, which would have
 * carried the signed rounding correction -- so only even ticks reach a case.
 * The four graphics cases differ in blob and destination cursor only; case 102
 * is the end, which turns the blend on hard and breaks the proc. The tail runs
 * on every tick including the default, so it sits after the switch rather than
 * in a default arm.
 *
 * The `(u16)` cast on the last argument is NOT decoration: the ROM narrows it
 * with `lsls #16; lsrs #16` and the shared prototype for SetBgScrollShadow declares
 * that parameter `u32`. Its third parameter is really `u16` -- see the note in
 * the wave report; the cast is here so this file does not have to retype
 * another agent's declaration. */

struct Unk08068BE4Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_08068AC5(struct Unk08068AC4 *proc)
{
    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    ResetWindowShadows();
    SetDefaultColorEffects();
    SetupBackgrounds(gUnknown_085813D4);
    gDispIo.disp_ct.obj_mapping = 1;
    SetDispEnable(1, 1, 0, 1, 0);
    gUnknown_0300251C.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_03002B6C.bits.priority = 3;
    ApplyPalettes(gUnknown_0817DA18, 1, 1);
    ApplyPalettes(gUnknown_0817DA18, 0x10, 1);
    Decompress(gUnknown_0817C408, (void *)0x06008000);
    sub_080718F8((u8 *)gBG3TilemapBuffer + 0x20C, gUnknown_0817D874, 0);
    BG_EnableSyncBG3();
    StartIntroBgJitter(0, proc);
    SetBgScrollShadow(2, 0, 0);
    SetBgScrollShadow(3, 0, 0);
    gDispIo.disp_ct.forced_blank = 0;
    proc->unk30 = 0;
    PlayMusic(0x12C);
}

void IntroT3_IDLE_08068BE5(struct Unk08068BE4Proc *proc)
{
    int t;

    t = 0xcc - proc->unk2c;
    switch (t >> 1)
    {
    case 15:
    case 18:
    case 21:
    case 31:
    case 34:
    case 37:
        sub_080718F8(gBG3TilemapBuffer + 0x106, gUnknown_0817D910, 0);
        BG_EnableSyncBG3();
        break;
    case 0:
    case 16:
    case 19:
    case 22:
    case 32:
    case 35:
    case 38:
        sub_080718F8(gBG3TilemapBuffer + 0x106, gUnknown_0817D874, 0);
        BG_EnableSyncBG3();
        break;
    case 52:
    case 55:
    case 58:
        sub_080718F8(gBG3TilemapBuffer + 0x104, gUnknown_0817D7B8, 0);
        BG_EnableSyncBG3();
        break;
    case 53:
    case 56:
    case 59:
        sub_080718F8(gBG3TilemapBuffer + 0x104, gUnknown_0817D6FC, 0);
        BG_EnableSyncBG3();
        break;
    case 102:
        StartBlendRampWhite8To0(0x20, 1, proc);
        REG_BLDCNT = 0xff;
        REG_BLDY = 0x1f;
        Proc_Break(proc);
        break;
    }
    SetBgScrollShadow(3, gUnknown_08581414[t % 5], (u16)gUnknown_08581414[t % 5 + 1]);
    proc->unk2c--;
}

asm(".global sub_08068AC4\n.thumb_set sub_08068AC4, IntroT3_08068AC5\n"
    ".global sub_08068BE4\n.thumb_set sub_08068BE4, IntroT3_IDLE_08068BE5\n");
