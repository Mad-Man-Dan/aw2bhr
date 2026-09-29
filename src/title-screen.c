#include "global.h"

#include "hardware.h"
#include "oam.h"
#include "proc.h"

struct ProcTitleScreen
{
    /* 00 */ PROC_HEADER;
    /* 2C */ s32 unk_2C;
    /* 30 */ s32 unk_30;
    /* 34 */ s32 unk_34;
};

struct ProcTitleSpriteDraw
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2C);
    /* 2C */ u32 unk_2C;
    /* 30 */ s32 unk_30;
    /* 34 */ u32 unk_34;
};

struct UnkProc08581CF8
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ u32 unk_34;
};

struct ProcTitlePressStartDraw
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2C);
    /* 2C */ u32 unk_2C;
    /* 30 */ u32 unk_30;
    /* 34 */ u32 unk_34;
    /* 38 */ STRUCT_PAD(0x38, 0x3C);
    /* 3C */ u32 unk_3C;
};

extern u32 gUnknown_0202F2C4;

extern s32 gGameClock;

extern u16 Pal_TitleScreenSprites[];
extern u16 Pal_TitleScreenBg[];
extern u8 Img_TitleScreenBg[];
extern u8 Img_0818DA8C[];

extern u8 Img_TitleScreenSprites[];

extern struct ProcCmd ProcScr_MainMenu[];

extern struct ProcCmd ProcScr_TitleToMainMenu[];
extern struct ProcCmd ProcScr_TitlePressStartDraw[];
extern struct ProcCmd ProcScr_TitleScreenSpriteDraw[];
extern struct ProcCmd ProcScr_TitleScreen[];

void TitleIntro_Init(struct ProcTitleScreen * proc)
{
    proc->unk_2C = 2700;
}
asm(".global sub_0806C9B4\n.thumb_set sub_0806C9B4, TitleIntro_Init\n");

void TitleIntro_WaitForInput(struct ProcTitleScreen * proc)
{
    if (sub_0803B18C() == 1)
    {
        return;
    }

    proc->unk_2C--;

    if (proc->unk_2C < 0)
    {
        ((struct ProcTitlePressStartDraw *)(Proc_Find(ProcScr_TitlePressStartDraw)))->unk_34 = 0;
        FadeOutMusicDefault();
        Proc_Break(proc);
    }

    if ((proc->unk_2C <= (2700 - gUnknown_0202F2C4)) && (gpKeySt->pressed & 9))
    {
        PlayMusicOrSfx2(0x71);
        LinkShutdown();
        Proc_GotoScript(proc, ProcScr_TitleToMainMenu);
    }
}
asm(".global sub_0806C9C0\n.thumb_set sub_0806C9C0, TitleIntro_WaitForInput\n");

void TitleIntro_StartFadeOut(struct ProcTitleScreen * proc)
{
    ColFadeToBlack(-1);
    proc->unk_30 = 0x20;
}
asm(".global sub_0806CA38\n.thumb_set sub_0806CA38, TitleIntro_StartFadeOut\n");

void TitleIntro_FadeOutLoop(struct ProcTitleScreen * proc)
{
    if (gGameClock & 1)
    {
        return;
    }

    sub_080718F0();
    EnablePaletteSync();

    proc->unk_30--;

    if (proc->unk_30 == 0)
    {
        Proc_Init();
        sub_0806A454();
    }
}
asm(".global sub_0806CA50\n.thumb_set sub_0806CA50, TitleIntro_FadeOutLoop\n");

// clang-format off

struct ProcCmd CONST_DATA ProcScr_TitleIntro[] =
{
    PROC_CALL(TitleIntro_Init),
    PROC_REPEAT(TitleIntro_WaitForInput),

    PROC_CALL(TitleIntro_StartFadeOut),
    PROC_REPEAT(TitleIntro_FadeOutLoop),

    PROC_END,
};

// clang-format on

void Title_StartMainMenu(void)
{
    Proc_Start(ProcScr_MainMenu, PROC_TREE_3);
}

// clang-format off

struct ProcCmd CONST_DATA ProcScr_TitleToMainMenu[] =
{
    PROC_29(2),
    PROC_1D(30),
    PROC_END_EACH(ProcScr_TitleScreen),
    PROC_CALL(Title_StartMainMenu),
    PROC_END,
};

// unknown - maybe "SetupBackgrounds" input?
u32 CONST_DATA gUnknown_08581CB8[] =
{
    0x06000000, 0x0600D800, 0x00000000, 0x00000001,
    0x06000000, 0x0600E000, 0x00000000, 0x00000000,
    0x06000000, 0x0600E800, 0x00000000, 0x00000000,
    0x06008000, 0x0600F000, 0x00000000, 0x00000000,
};

// clang-format on

void TitleScreen_Init(struct ProcTitleScreen * proc)
{
    SetupBackgrounds(gUnknown_08581CB8);

    gDispIo.disp_ct.mode = 0;
    SetDispEnable(1, 0, 0, 0, 1);
    /* Through the union member agbcc swaps the operands of the `orr` and the
     * two bytes stop matching; a plain u8 lvalue keeps the ROM's order. */
    *(u8 *)&gUnknown_03002B6C |= 0x80;
    gDispIo.disp_ct.obj_mapping = 1;

    ResetWindowShadows();
    SetDefaultColorEffects();

    ResetBgAffineToScreenCentre();

    ApplyPaletteExt(Pal_TitleScreenSprites, 0x200, 0xA0);
    ApplyPaletteExt(Pal_TitleScreenBg, 0, 0x200);
    Decompress(Img_TitleScreenBg, (void *)0x06000000);
    Decompress(Img_0818DA8C, (void *)0x0600D800);

    LoadTitleScreenSpriteGraphics();

    proc->unk_30 = 127;

    gDispIo.disp_ct.forced_blank = 0;

    if (proc->unk_34 != 0)
    {
        SetBgScrollShadow(0, 0x40, 0);
        return;
    }

    Proc_Goto(proc, 0);
}
asm(".global sub_0806CA98\n.thumb_set sub_0806CA98, TitleScreen_Init\n");

void TitleScreen_LoopStatic(struct ProcTitleScreen * proc)
{
    if (proc->unk_30 != 0)
    {
        ((struct ProcTitleSpriteDraw *)(Proc_Start(ProcScr_TitleScreenSpriteDraw, proc)))->unk_34 = proc->unk_34;
        FadeOutMusicDefault();
    }

    proc->unk_30 = 0;
    sub_0806CC4C();
}
asm(".global sub_0806CB5C\n.thumb_set sub_0806CB5C, TitleScreen_LoopStatic\n");

void TitleScreen_StartScrollIn(struct ProcTitleScreen * proc)
{
    ColFadeFromWhite(-1);
    sub_080718F0();
    EnablePaletteSync();
    proc->unk_30 = 127;
}
asm(".global sub_0806CB88\n.thumb_set sub_0806CB88, TitleScreen_StartScrollIn\n");

void TitleScreen_LoopScrollIn(struct ProcTitleScreen * proc)
{
    if (proc->unk_30 >= 0)
    {
        SetBgScrollShadow(0, 64 - (proc->unk_30 >> 1), 0);

        if ((proc->unk_30 & 3) == 2)
        {
            sub_080718F0();
            EnablePaletteSync();
        }

        proc->unk_30--;
    }

    if (proc->unk_30 == 10)
    {
        ((struct ProcTitleSpriteDraw *)(Proc_Start(ProcScr_TitleScreenSpriteDraw, proc)))->unk_34 = proc->unk_34;
        FadeOutMusicDefault();
    }

    sub_0806CC4C();
}
asm(".global sub_0806CBA8\n.thumb_set sub_0806CBA8, TitleScreen_LoopScrollIn\n");

// clang-format off

struct ProcCmd CONST_DATA ProcScr_TitleScreen[] =
{
    PROC_YIELD,
    PROC_START_BUGGED(ProcScr_TitleIntro, 3),

    PROC_CALL(TitleScreen_Init),
    PROC_REPEAT(TitleScreen_LoopStatic),

PROC_LABEL(0),
    PROC_CALL(TitleScreen_StartScrollIn),
    PROC_REPEAT(TitleScreen_LoopScrollIn),

    PROC_END,
};

// clang-format on

void StartTitleScreen(s32 arg0)
{
    ((struct UnkProc08581CF8 *)(Proc_Start(ProcScr_TitleScreen, PROC_TREE_3)))->unk_34 = arg0;

    if (arg0 != 0)
    {
        gUnknown_0202F2C4 = 180;
    }
    else
    {
        gUnknown_0202F2C4 = 360;
    }
}
asm(".global sub_0806CC00\n.thumb_set sub_0806CC00, StartTitleScreen\n");

bool8 IsTitleScreenRunning(void)
{
    return Proc_Exists(ProcScr_TitleScreen);
}
asm(".global sub_0806CC34\n.thumb_set sub_0806CC34, IsTitleScreenRunning\n");

// clang-format off

u16 CONST_DATA Sprite_Title_Number2[] =
{
    1,
    OAM0_SHAPE_64x64, OAM1_SIZE_64x64, 0,
};

u16 CONST_DATA Sprite_08581D40[] =
{
    5,
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16, OAM2_CHR(0x148) + OAM2_PAL(3),
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16 + OAM1_X(+32), OAM2_CHR(0x150) + OAM2_PAL(3),
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16 + OAM1_X(+64), OAM2_CHR(0x158) + OAM2_PAL(3),
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16 + OAM1_X(+96), OAM2_CHR(0x160) + OAM2_PAL(3),
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16 + OAM1_X(+128), OAM2_CHR(0x168) + OAM2_PAL(3),
};

// clang-format on

void sub_0806CC4C(void)
{
    PutOamHi(40, 140, Sprite_08581D40, 0);
}

void LoadTitleScreenSpriteGraphics(void)
{
    Decompress(Img_TitleScreenSprites, (void *)0x06010000);
}
asm(".global sub_0806CC64\n.thumb_set sub_0806CC64, LoadTitleScreenSpriteGraphics\n");

// clang-format off

u16 CONST_DATA Sprite_Title_BlackHoleRising[] =
{
    11,
    OAM0_SHAPE_64x32, OAM1_SIZE_64x32, OAM2_CHR(0x40) + OAM2_PAL(2),
    OAM0_SHAPE_64x32, OAM1_SIZE_64x32 + OAM1_X(+64), OAM2_CHR(0x60) + OAM2_PAL(2),
    OAM0_SHAPE_64x32, OAM1_SIZE_64x32 + OAM1_X(+128), OAM2_CHR(0x80) + OAM2_PAL(2),
    OAM0_SHAPE_32x32, OAM1_SIZE_32x32 + OAM1_X(+192), OAM2_CHR(0xA0) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16, OAM2_CHR(0xB0) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16 + OAM1_X(+32), OAM2_CHR(0xB8) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16 + OAM1_X(+64), OAM2_CHR(0xC0) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16 + OAM1_X(+96), OAM2_CHR(0xC8) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16 + OAM1_X(+128), OAM2_CHR(0xD0) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16 + OAM1_X(+160), OAM2_CHR(0xD8) + OAM2_PAL(2),
    OAM0_SHAPE_32x16 + OAM0_Y(+32), OAM1_SIZE_32x16 + OAM1_X(+192), OAM2_CHR(0xE0) + OAM2_PAL(2),
};

u16 CONST_DATA Sprite_Title_AdvanceWars[] =
{
    3,
    OAM0_SHAPE_64x32, OAM1_SIZE_64x32, OAM2_CHR(0xE8) + OAM2_PAL(1),
    OAM0_SHAPE_64x32, OAM1_SIZE_64x32 + OAM1_X(+64), OAM2_CHR(0x108) + OAM2_PAL(1),
    OAM0_SHAPE_64x32, OAM1_SIZE_64x32 + OAM1_X(+128), OAM2_CHR(0x128) + OAM2_PAL(1),
};

u16 CONST_DATA Padding_08581DB8[] = { 0 }; // TODO: padding?

u16 CONST_DATA gUnknown_08581DBA[] =
{
    8,
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8, OAM2_CHR(0x1C0),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+16), OAM2_CHR(0x1C2),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+32), OAM2_CHR(0x1C4),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+48), OAM2_CHR(0x1C6),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+64) + OAM1_HFLIP, OAM2_CHR(0x1C6),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+80) + OAM1_HFLIP, OAM2_CHR(0x1C4),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+96) + OAM1_HFLIP, OAM2_CHR(0x1C2),
    OAM0_SHAPE_16x8 + OAM0_BLEND, OAM1_SIZE_16x8 + OAM1_X(+112) + OAM1_HFLIP, OAM2_CHR(0x1C0),
};

u16 CONST_DATA Sprite_Title_PressStart[] =
{
    4,
    OAM0_SHAPE_32x16 + OAM0_BLEND, OAM1_SIZE_32x16, OAM2_CHR(0x170) + OAM2_PAL(3),
    OAM0_SHAPE_32x16 + OAM0_BLEND, OAM1_SIZE_32x16 + OAM1_X(+32), OAM2_CHR(0x178) + OAM2_PAL(3),
    OAM0_SHAPE_32x16 + OAM0_BLEND, OAM1_SIZE_32x16 + OAM1_X(+64), OAM2_CHR(0x180) + OAM2_PAL(3),
    OAM0_SHAPE_32x16 + OAM0_BLEND, OAM1_SIZE_32x16 + OAM1_X(+96), OAM2_CHR(0x188) + OAM2_PAL(3),
};

u16 CONST_DATA Sprite_Title_Trademark[] =
{
    1,
    OAM0_SHAPE_16x8, OAM1_SIZE_16x8, OAM2_CHR(0x190) + OAM2_PAL(1),
};

// clang-format on

void TitlePressStartDraw_Init(struct ProcTitlePressStartDraw * proc)
{
    (&gDispIo.blend_ct)->effect = 1; // TODO: macro?
    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(1, 1, 1, 1, 0);

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 16;

    proc->unk_34 = 0;
    proc->unk_2C = 0;
    proc->unk_30 = 0;
    proc->unk_3C = 0;
}

void TitlePressStartDraw_Loop(struct ProcTitlePressStartDraw * proc)
{
    s32 var_r2;

    s32 r1 = proc->unk_2C--;

    if (!(r1 % 4) && (proc->unk_34 != 0))
    {
        PAL_OBJ_COLOR(3, 12) = PAL_OBJ_COLOR(4, proc->unk_3C);
        EnablePaletteSync();
        proc->unk_3C = ((proc->unk_3C + 1) & 15);
    }

    if (proc->unk_34 != 0)
    {
        proc->unk_30++;
        var_r2 = ((s32)(proc->unk_30 + (proc->unk_30 >> 31)) >> 1) & 63;

        if (var_r2 > 31)
        {
            var_r2 = 64 - var_r2;
        }

        if (var_r2 > 16)
        {
            var_r2 = 16;
        }

        gUnknown_03002020 = var_r2;
        gUnknown_03002B28 = 16 - var_r2;
    }

    PutSprite(0, 56, 100, Sprite_Title_PressStart, 0);
}

// clang-format off

struct ProcCmd CONST_DATA ProcScr_TitlePressStartDraw[] =
{
    PROC_CALL(TitlePressStartDraw_Init),
    PROC_REPEAT(TitlePressStartDraw_Loop),

    PROC_END,
};

// clang-format on

void TitleSpriteDraw_Init(struct ProcTitleSpriteDraw * proc)
{
    proc->unk_30 = 0;

    if (proc->unk_34 != 0)
    {
        Proc_Goto(proc, 0);
        return;
    }

    PlayMusicOrSfx2(0x1CF);
}

void TitleSpriteDraw_Loop_Draw2(struct ProcTitleSpriteDraw * proc)
{
    s32 temp_r0 = Interpolate(0, 512, 256, proc->unk_30, 20);

    // clang-format off
    SetObjAffine(
        0,
        Div(+COS_Q12(0) << 4, temp_r0),
        Div(-SIN_Q12(0) << 4, temp_r0),
        Div(+SIN_Q12(0) << 4, temp_r0),
        Div(+COS_Q12(0) << 4, temp_r0)
    );
    // clang-format on

    PutSpriteExt(0, 56, 992, Sprite_Title_Number2, 0);

    proc->unk_30++;

    if (proc->unk_30 == 20)
    {
        proc->unk_30 = 0;
        Proc_Break(proc);
    }
}

void TitleSpriteDraw_Loop_SlideTitleIn(struct ProcTitleSpriteDraw * proc)
{
    s32 var_r8 = 88;

    s32 temp_r7 = Interpolate(1, 240, 22, proc->unk_30, 22);
    s32 temp_r6 = Interpolate(1, -240, 8, proc->unk_30, 22);

    if (proc->unk_30 > 13)
    {
        var_r8 = Interpolate(0, 88, 176, proc->unk_30 - 14, 8);
    }

    PutSprite(0, temp_r6, 16, Sprite_Title_BlackHoleRising, 0);
    PutSprite(0, var_r8, 0, Sprite_Title_Number2, 0);
    PutSprite(0, temp_r7, 15, Sprite_Title_AdvanceWars, 0);

    if (proc->unk_30 == 22)
    {
        PlayMusicOrSfx2(0x79);
        Proc_Break(proc);
    }

    proc->unk_30++;
}

void TitleSpriteDraw_StartIdle(struct ProcTitleSpriteDraw * proc)
{
    proc->unk_30 = 32;
    proc->unk_2C = 173;

    ColorFadeInit();
    ColFadeFromWhite(-1);
    sub_080718F0();
    EnablePaletteSync();
}

void TitleSpriteDraw_Loop_Idle(struct ProcTitleSpriteDraw * proc)
{
    if (!(gGameClock & 3) && (proc->unk_30 > 0))
    {
        sub_080718F0();
        EnablePaletteSync();
        proc->unk_30--;
    }

    if (proc->unk_30 == 0)
    {
        ((struct ProcTitlePressStartDraw *)(Proc_Find(ProcScr_TitlePressStartDraw)))->unk_34 = 1;
        PlayMusic(0x1AA);
        proc->unk_30 = -1;
    }

    PutSprite(0, 8, 16, Sprite_Title_BlackHoleRising, 0);
    PutSprite(0, 176, 0, Sprite_Title_Number2, 0);
    PutSprite(0, 22, 15, Sprite_Title_AdvanceWars, 0);
    PutSprite(0, 202, 34, Sprite_Title_Trademark, 0);
}

// clang-format off

struct ProcCmd CONST_DATA ProcScr_TitleScreenSpriteDraw[] =
{
    PROC_YIELD,

    PROC_CALL(TitleSpriteDraw_Init),

    PROC_REPEAT(TitleSpriteDraw_Loop_Draw2),
    PROC_REPEAT(TitleSpriteDraw_Loop_SlideTitleIn),

PROC_LABEL(0),
    PROC_START_CHILD(ProcScr_TitlePressStartDraw),

    PROC_CALL(TitleSpriteDraw_StartIdle),
    PROC_REPEAT(TitleSpriteDraw_Loop_Idle),

    PROC_END,
};

// clang-format on
