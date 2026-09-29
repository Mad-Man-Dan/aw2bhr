#include "global.h"
#include "proc.h"
#include "hardware.h"

void ClearMainLoopFrameMask(void)
{
    gUnknown_030043F4 = 0;
}
asm(".global sub_08036B28\n.thumb_set sub_08036B28, ClearMainLoopFrameMask\n");

void ClearMainLoopFrameMaskAndEnableSpriteLayer(void)
{
    gUnknown_030043F4 = 0;
    EnableSpriteLayerMode();
}
asm(".global sub_08036B34\n.thumb_set sub_08036B34, ClearMainLoopFrameMaskAndEnableSpriteLayer\n");

static void sub_08036B48(void)
{
    for (;;)
        ;
}

void InitGameSystems(void)
{
    gUnknown_030040A0 = 0;
    gUnknown_02028E40 = 0;
    ForceScreenBlack();
    sub_080366C4(0);
    sub_080366D0(0);
    gUnknown_03004094 = 0;
    gGameClock = 0;
    gUnknown_03003330 = 0;
    gUnknown_03004078 = 0;
    gUnknown_030043F0 = 0;
    gUnknown_030033F0 = 0;
    ClearMainLoopFrameMaskAndEnableSpriteLayer();
    StoreRoutinesToIRAM();
    InitRecordListPointersAndTerrainTable();
    InitVersusPlayState();
    InitSpriteEngine();
    ClearAllSpriteScripts2();
    ClearTileRigistry();
    sub_08011B18();
    ClearVBlankCallbackQueue();
    sub_080191B0();
    InitSlotScripts();
    Proc_Init();
    ClearMoveSlideSlots();
    InitScreenFadeLatch();
    InitKeySt();
    InitMapFloodHandler();
    EnableVBlankInterrupt();
    FlushLCDControl();
    InitSoundSystem();
    InitSoundMode();
    LinkShutdown();
    sub_08085AF4();
    ResetMainMenuCarouselState(0);
    sub_0801F114();
}
asm(".global sub_08036B4C\n.thumb_set sub_08036B4C, InitGameSystems\n");

void InstallQueuedSpritesFrameCallbacks(void)
{
    DisableSpriteLayerMode();
    sub_080366D0(QueuedSpritesVBlankCallback);
    sub_080366C4(QueuedSpritesMainLoopCallback);
    ClearMainLoopFrameMask();
}
asm(".global sub_08036C08\n.thumb_set sub_08036C08, InstallQueuedSpritesFrameCallbacks\n");

void StartBattleAnimParamMenu(void)
{
    InitGameSystems();
    InstallQueuedSpritesFrameCallbacks();
    Proc_Start(gUnknown_08553754, PROC_TREE_3);
    ClearMainLoopFrameMaskAndEnableSpriteLayer();
}
asm(".global sub_08036C2C\n.thumb_set sub_08036C2C, StartBattleAnimParamMenu\n");

void BootToIntroSequence(void)
{
    EnableSpriteLayerMode();
    InitGameSystems();
    gUnknown_030032CC = 0xE28;
    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
    sub_0806A454();
}
asm(".global sub_08036C4C\n.thumb_set sub_08036C4C, BootToIntroSequence\n");

void BootToIntroSequence2(void)
{
    EnableSpriteLayerMode();
    InitGameSystems();
    gUnknown_030032CC = 0xE28;
    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
    sub_0806A454();
}
asm(".global sub_08036C80\n.thumb_set sub_08036C80, BootToIntroSequence2\n");

void ClearWorkRamAndSoftReset(void)
{
    u8 buf[4];
    vu16 fill;
    vu32 *dma;

    buf[0] = gUnknown_02028E41[0];
    buf[1] = gUnknown_02028E41[1];
    buf[2] = gUnknown_02028E41[2];
    buf[3] = gUnknown_02028E41[3];
    fill = 0;
    dma = (vu32 *)(REG_BASE + REG_OFFSET_DMA3SAD);
    dma[0] = (u32)&fill;
    dma[1] = 0x02000000;
    dma[2] = 0x81020000;
    dma[2];
    gUnknown_02028E41[0] = buf[0];
    gUnknown_02028E41[1] = buf[1];
    gUnknown_02028E41[2] = buf[2];
    gUnknown_02028E41[3] = buf[3];
    SoftReset(0xFE);
}
asm(".global sub_08036CB4\n.thumb_set sub_08036CB4, ClearWorkRamAndSoftReset\n");

void AgbMain(void)
{
    u32 zero;
    volatile u16 keys;
    vu32 *dma;
    int flag;

    zero = 0;
    dma = (vu32 *)(REG_BASE + REG_OFFSET_DMA3SAD);
    dma[0] = (u32)&zero;
    dma[1] = 0x03000000;
    dma[2] = 0x85001FE0;
    dma[2];
    REG_WAITCNT = 0x45B4;
    keys = ~REG_KEYINPUT & 0x3FF;
    StoreIRQToIRAM();
    if (HeapInit(gUnknown_02003000, 0x8000) == -1)
        sub_08036B48();
    InitSaveSystem(PackProfileRecord, ResetProfileToDefaults, gUnknown_02000000, 2, gUnknown_03003064);
    LoadProfile();
    sub_0803D48C();
    SetRandomSeed(0x0A6B99CD);
    sub_080128C4();
    FlushLCDControl();
    SetIRQHandler(0, sub_080366F4);
    flag = (keys & 0xF) != 0xF && keys == 0x214;
    if (flag)
        StartEraseSaveDataScreen();
    else
        BootToIntroSequence();
    UpdateInterruptEnable(2, 0x00012001);
    for (;;) {
        if (gUnknown_030040EC != 0)
            gUnknown_030040EC();
        CheckSoftResetCombo();
    }
}

void CheckSoftResetCombo(void)
{
    u16 keys;

    keys = ~REG_KEYINPUT & 0x3ff;

    if ((keys & 0xf) == 0xf)
    {
        if (gUnknown_02028E41[0] != 0xaa || gUnknown_02028E41[1] != 0x55)
            SetLanguageSignature();

        ClearWorkRamAndSoftReset();
    }
}

asm(".global sub_08036E18\n.thumb_set sub_08036E18, CheckSoftResetCombo\n");

void StartEraseSaveDataScreen(void)
{
    InitGameSystems();
    InitTextTileCache(0);
    sub_080152EC(gUnknown_0849D1AC, 0);
}
asm(".global sub_08036E54\n.thumb_set sub_08036E54, StartEraseSaveDataScreen\n");

void EraseSaveScreen_Init(void)
{
    SetupBackgrounds(gUnknown_0849D16C);
    gDispIo.disp_ct.forced_blank = 0;
    EnableVBlankInterrupt();
    FlushLCDControl();
    CpuCopyAuto(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    CpuCopyAuto(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    CpuCopyAuto(gBG2TilemapBuffer, (void *)0x06007800, 0x800);
    CpuCopyAuto(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);
    SetMapLayerPrioritiesDefault();
    LoadBg1WindowFrame(0);
    ApplyPaletteExt(gUnknown_0809165C, 0x140, 0x20);
    sub_08011B18();
    sub_080366C4(DefaultMainLoopCallback);
    sub_080366D0(DefaultVBlankCallback);
}
asm(".global sub_08036E70\n.thumb_set sub_08036E70, EraseSaveScreen_Init\n");

void EraseSaveScreen_OpenPrompt(void)
{
    gUnknown_02028E40 = gUnknown_0200C420.unk08 >> 6;
    StartEventScript(gUnknown_0849D34C);
}
asm(".global sub_08036F20\n.thumb_set sub_08036F20, EraseSaveScreen_OpenPrompt\n");

void sub_08036F44(void)
{
    gDispIo.disp_ct.bg0_enable = 1;
    gDispIo.disp_ct.bg2_enable = 1;

    LoadCursorSpriteGraphics();
    LoadBg1WindowFrame(0);
}

void StartBattleAnimScene(u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8,
                  u8 a9, u8 a10, u8 a11, u8 a12, u8 a13, u8 a14, u8 a15,
                  u8 a16, u16 a17)
{
    gUnknown_03002B5C = 0;
    gUnknown_0300450C = a15;

    gUnknown_03004580[0][0] = a2 - 1;
    gUnknown_03004580[1][0] = a9 - 1;
    gUnknown_03004580[0][1] = a4 - 1;
    gUnknown_03004580[1][1] = a11 - 1;
    gUnknown_03004580[0][2] = a5;
    gUnknown_03004580[1][2] = a12;
    gUnknown_03004580[0][3] = a3;
    gUnknown_03004580[1][3] = a10;
    gUnknown_03004580[0][4] = a1;
    gUnknown_03004580[1][4] = a8;
    gUnknown_03004580[0][5] = a6;
    gUnknown_03004580[1][5] = a13;
    gUnknown_03004580[0][6] = a7;
    gUnknown_03004580[1][6] = a14;
    gUnknown_03004580[0][7] = gUnknown_085D583C[a3].defense * 10;
    gUnknown_03004580[1][7] = gUnknown_085D583C[a10].defense * 10;

    gUnknown_02027F68[1] = 0;
    gUnknown_03004528[0] = gUnknown_02027F68;
    gUnknown_03004528[1] = gUnknown_02027F68;
    gUnknown_03004520 = a16;

    SetBattleAnimFlagsForGame();

    gUnknown_03004504.bit0 = 1;
    gUnknown_03004504.bit1 = 0;
    gUnknown_03004504.bit2 = 0;
    gUnknown_03004504.bit3 = 0;
    gUnknown_03004504.bit4 = 0;
    gUnknown_03004504.bit5 = 0;
    gUnknown_03004504.bit6 = 0;
    gUnknown_03004504.unk02 = a17;

    Proc_Start(gUnknown_0849D3BC, PROC_TREE_3);
}
asm(".global sub_08036F68\n.thumb_set sub_08036F68, StartBattleAnimScene\n");

void SetDefaultFrameCallbacks(void)
{
    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
}
asm(".global sub_080370F0\n.thumb_set sub_080370F0, SetDefaultFrameCallbacks\n");

int IsBattleAnimSceneRunning(void)
{
    return Proc_Find(gUnknown_0849D3BC) != 0;
}
asm(".global sub_0803710C\n.thumb_set sub_0803710C, IsBattleAnimSceneRunning\n");

void EndBattleAnimScene(void)
{
    EndAllSpriteScripts();
    sub_0801537C(gUnknown_08553820);
    Proc_EndEach(gUnknown_0855379C);
    Proc_EndEach(gUnknown_0849D3BC);
    ClearMainLoopFrameMaskAndEnableSpriteLayer();
}
asm(".global sub_08037124\n.thumb_set sub_08037124, EndBattleAnimScene\n");

void sub_08037150(int a)
{
    Decompress(gUnknown_08124478, (u8 *)OBJ_VRAM0 + (u16)(a * 0x40) / 2);
}
