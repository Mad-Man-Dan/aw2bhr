#include "global.h"
#include "proc.h"
#include "hardware.h"

void sub_08036B28(void)
{
    gUnknown_030043F4 = 0;
}

void sub_08036B34(void)
{
    gUnknown_030043F4 = 0;
    sub_0801F00C();
}

static void sub_08036B48(void)
{
    for (;;)
        ;
}

void sub_08036B4C(void)
{
    gUnknown_030040A0 = 0;
    gUnknown_02028E40 = 0;
    sub_08010FE0();
    sub_080366C4(0);
    sub_080366D0(0);
    gUnknown_03004094 = 0;
    gGameClock = 0;
    gUnknown_03003330 = 0;
    gUnknown_03004078 = 0;
    gUnknown_030043F0 = 0;
    gUnknown_030033F0 = 0;
    sub_08036B34();
    StoreRoutinesToIRAM();
    sub_0803486C();
    sub_08034848();
    sub_0801BCE0();
    sub_08015544();
    ClearTileRigistry();
    sub_08011B18();
    sub_08011A84();
    sub_080191B0();
    sub_08015184();
    Proc_Init();
    sub_08035568();
    sub_08010F94();
    sub_08013434();
    sub_0801F4A4();
    sub_0801295C();
    sub_080128D0();
    sub_0803B37C();
    sub_0803B688();
    sub_08030ED4();
    sub_08085AF4();
    sub_08080F90(0);
    sub_0801F114();
}

void sub_08036C08(void)
{
    sub_0801F018();
    sub_080366D0(sub_08036A50);
    sub_080366C4(sub_08036AB8);
    sub_08036B28();
}

void sub_08036C2C(void)
{
    sub_08036B4C();
    sub_08036C08();
    Proc_Start(gUnknown_08553754, PROC_TREE_3);
    sub_08036B34();
}

void sub_08036C4C(void)
{
    sub_0801F00C();
    sub_08036B4C();
    gUnknown_030032CC = 0xE28;
    sub_080366D0(sub_08036884);
    sub_080366C4(sub_080368E8);
    sub_0806A454();
}

void sub_08036C80(void)
{
    sub_0801F00C();
    sub_08036B4C();
    gUnknown_030032CC = 0xE28;
    sub_080366D0(sub_08036884);
    sub_080366C4(sub_080368E8);
    sub_0806A454();
}

void sub_08036CB4(void)
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
    if (sub_08014DA8(gUnknown_02003000, 0x8000) == -1)
        sub_08036B48();
    InitSaveSystem(PackProfileRecord, ResetProfileToDefaults, gUnknown_02000000, 2, gUnknown_03003064);
    LoadProfile();
    sub_0803D48C();
    sub_080129D4(0x0A6B99CD);
    sub_080128C4();
    sub_080128D0();
    SetIRQHandler(0, sub_080366F4);
    flag = (keys & 0xF) != 0xF && keys == 0x214;
    if (flag)
        sub_08036E54();
    else
        sub_08036C4C();
    sub_0801BB10(2, 0x00012001);
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
            sub_0804A010();

        sub_08036CB4();
    }
}

asm(".global sub_08036E18\n.thumb_set sub_08036E18, CheckSoftResetCombo\n");

void sub_08036E54(void)
{
    sub_08036B4C();
    sub_0801B780(0);
    sub_080152EC(gUnknown_0849D1AC, 0);
}

void sub_08036E70(void)
{
    sub_08012C58(gUnknown_0849D16C);
    gDispIo.disp_ct.forced_blank = 0;
    sub_0801295C();
    sub_080128D0();
    sub_08011C68(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    sub_08011C68(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    sub_08011C68(gBG2TilemapBuffer, (void *)0x06007800, 0x800);
    sub_08011C68(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);
    sub_0802465C();
    sub_0801A5B0(0);
    ApplyPaletteExt(gUnknown_0809165C, 0x140, 0x20);
    sub_08011B18();
    sub_080366C4(sub_080368E8);
    sub_080366D0(sub_08036884);
}

void sub_08036F20(void)
{
    gUnknown_02028E40 = gUnknown_0200C420.unk08 >> 6;
    StartEventScript(gUnknown_0849D34C);
}

void sub_08036F44(void)
{
    gDispIo.disp_ct.bg0_enable = 1;
    gDispIo.disp_ct.bg2_enable = 1;

    sub_08022A34();
    sub_0801A5B0(0);
}

void sub_08036F68(u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8,
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

    sub_080546BC();

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

void sub_080370F0(void)
{
    sub_080366D0(sub_08036884);
    sub_080366C4(sub_080368E8);
}

int sub_0803710C(void)
{
    return Proc_Find(gUnknown_0849D3BC) != 0;
}

void sub_08037124(void)
{
    sub_080169E8();
    sub_0801537C(gUnknown_08553820);
    Proc_EndEach(gUnknown_0855379C);
    Proc_EndEach(gUnknown_0849D3BC);
    sub_08036B34();
}

void sub_08037150(int a)
{
    Decompress(gUnknown_08124478, (u8 *)OBJ_VRAM0 + (u16)(a * 0x40) / 2);
}
