#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803670C.
 * sub_0803670C @ 0x0803670C, UpdateFuelAmmoGraphics @ 0x0803678C, sub_08036884 @ 0x08036884, sub_080368E8 @ 0x080368E8, sub_08036944 @ 0x08036944, sub_080369BC @ 0x080369BC, sub_08036A50 @ 0x08036A50, sub_08036AB8 @ 0x08036AB8
 */

#include "hardware.h"
#include "proc.h"

void sub_0803670C(void)
{
    u32 v;

    v = (u32)gGameClock % 0x2e;

    if (v <= 0xb)
        v = 0;
    else if (v > 0x11)
    {
        if (v <= 0x27)
            v = 2;
        else
            v = 1;
    }
    else
    {
        v = 1;
    }

    CpuCopyAuto(gUnknown_081120B0 + (v & 0x3ff) * 32,
                 (u8 *)(gUnknown_03002B6C.bits.chr_block * 0x4000) + 0x06004100,
                 0x20);
    CpuCopyAuto(gUnknown_081120B0 + ((v * 2 + 3) & 0x3ff) * 32,
                 (u8 *)(gUnknown_03002B6C.bits.chr_block * 0x4000) + 0x06004120,
                 0x40);
}

/* Named per Xenesis's AW2 Subroutine List: "Subroutine for fuel/ammo
 * graphics updates. Updates on Frame 0, 20, 40" -- matches the switch below
 * exactly (0x14=20, 0x28=40). The old UpdateFuelAmmoGraphics symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged. */
void UpdateFuelAmmoGraphics(void)
{
    u32 v;

    v = (u32)gGameClock % 0x32;

    switch (v)
    {
    case 0:
        CpuCopyAuto(gUnknown_081251B0,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067a0,
                     0x20);
        CpuCopyAuto(gUnknown_081251B0 + 0x20,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067c0,
                     0x20);
        CpuCopyAuto(gUnknown_081251B0 + 0x20,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067e0,
                     0x20);
        break;

    case 0x14:
        CpuCopyAuto(gUnknown_081251B0,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067e0,
                     0x20);
        break;

    case 0x28:
        CpuCopyAuto(gUnknown_08090EC4,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067a0,
                     0x20);
        CpuCopyAuto(gUnknown_08090EC4,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067c0,
                     0x20);
        CpuCopyAuto(gUnknown_08090EC4,
                     (u8 *)(gUnknown_030030B4.bits.chr_block * 0x4000) + 0x060067e0,
                     0x20);
        break;
    }
}

asm(".global sub_0803678C\n.thumb_set sub_0803678C, UpdateFuelAmmoGraphics\n");

void DefaultVBlankCallback(void)
{
    RunSoundVSync();
    sub_0802FACC();
    Proc_Run(gProcTreeRootArray[0]);
    RunVBlankHooks();
    SyncLoOamForMode();
    ClearLoOamForMode();
    if (gUnknown_03004094 != 0)
    {
        gUnknown_03004094 = 0;
        SyncHiOamForMode();
        FlushLCDControl();
        FlushTiles();
        FlushBgTilemaps();
        RunVBlankCallbackQueue();
    }
    else
    {
        gUnknown_03004094 = 0;
    }
    TickSimpleSpriteScriptsForMode();
    gGameClock++;
    RunSoundMain();
}
asm(".global sub_08036884\n.thumb_set sub_08036884, DefaultVBlankCallback\n");

void DefaultMainLoopCallback(void)
{
    if (gUnknown_03004094 == 0)
    {
        BeginOamFrameForMode();
        RefreshKeySt();
        LatchBattleAnimKeys();
        RunEventScripts();
        RunAllSlotScripts();
        Proc_Run(gProcTreeRootArray[1]);
        Proc_Run(gProcTreeRootArray[2]);
        Proc_Run(gProcTreeRootArray[3]);
        Proc_Run(gProcTreeRootArray[5]);
        Proc_Run(gProcTreeRootArray[4]);
        DrawSimpleSpriteScriptsForMode();
        FlushSpritesForMode();
        SoundMainLoopNoOp();
        gUnknown_03004094 = 1;
    }
}
asm(".global sub_080368E8\n.thumb_set sub_080368E8, DefaultMainLoopCallback\n");

void MapVBlankCallback(void)
{
    gUnknown_030044D0 = 1;
    RunSoundVSync();
    sub_0802FACC();
    Proc_Run(gProcTreeRootArray[0]);
    RunVBlankHooks();
    SyncLoOamForMode();
    ClearLoOamForMode();
    if (gUnknown_03004094 != 0)
    {
        gUnknown_03004094 = 0;
        SyncHiOamForMode();
        FlushLCDControl();
        FlushTiles();
        FlushBgTilemaps();
        RunVBlankCallbackQueue();
    }
    else
    {
        gUnknown_03004094 = 0;
    }
    TickSimpleSpriteScriptsForMode();
    gGameClock++;
    RunSoundMain();
    gUnknown_030044D0 = 0;
}
asm(".global sub_08036944\n.thumb_set sub_08036944, MapVBlankCallback\n");

void MapMainLoopCallback(void)
{
    if (gUnknown_03004094 == 0 && (gGameClock & gUnknown_030043F4) == 0)
    {
        BeginOamFrameForMode();
        RefreshKeySt();
        LatchBattleAnimKeys();
        RunEventScripts();

        if (gUnknown_03003F3C != 0)
        {
            if (gUnknown_03003F3C == 1)
                RunMapStateMachine();
        }

        RunAllSlotScripts();
        Proc_Run(gProcTreeRootArray[1]);
        Proc_Run(gProcTreeRootArray[2]);
        Proc_Run(gProcTreeRootArray[3]);
        Proc_Run(gProcTreeRootArray[5]);
        Proc_Run(gProcTreeRootArray[4]);
        UpdateMapDisplay();
        DrawMapObjectSprites();
        DrawSimpleSpriteScriptsForMode();
        FlushSpritesForMode();
        SoundMainLoopNoOp();
        gUnknown_03004094 = 1;
    }
}
asm(".global sub_080369BC\n.thumb_set sub_080369BC, MapMainLoopCallback\n");

void QueuedSpritesVBlankCallback(void)
{
    RunSoundVSync();
    sub_0802FACC();
    Proc_Run(gProcTreeRootArray[0]);
    RunVBlankHooks();
    SyncLoOamForMode();
    ClearLoOamForMode();
    if (gUnknown_03004094 != 0)
    {
        gUnknown_03004094 = 0;
        SyncHiOamForMode();
        FlushLCDControl();
        FlushTiles();
        FlushBgTilemaps();
        RunVBlankCallbackQueue();
        BeginOamFrameForMode();
    }
    else
    {
        gUnknown_03004094 = 0;
    }
    TickSimpleSpriteScriptsForMode();
    gGameClock++;
    RunSoundMain();
}
asm(".global sub_08036A50\n.thumb_set sub_08036A50, QueuedSpritesVBlankCallback\n");

void QueuedSpritesMainLoopCallback(void)
{
    if (gUnknown_03004094 == 0 && (gGameClock & gUnknown_030043F4) == 0)
    {
        RefreshKeySt();
        LatchBattleAnimKeys();
        RunEventScripts();
        Proc_Run(gProcTreeRootArray[1]);
        Proc_Run(gProcTreeRootArray[2]);
        Proc_Run(gProcTreeRootArray[3]);
        RunAllSlotScripts();
        Proc_Run(gProcTreeRootArray[5]);
        Proc_Run(gProcTreeRootArray[4]);
        DrawSimpleSpriteScriptsForMode();
        FlushSpritesForMode();
        SoundMainLoopNoOp();
        gUnknown_03004094 = 1;
    }
}
asm(".global sub_08036AB8\n.thumb_set sub_08036AB8, QueuedSpritesMainLoopCallback\n");
