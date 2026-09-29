#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080489CC.
 * sub_080489CC @ 0x080489CC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"

/* MATCHED (wave 60, W60-E), 1088 bytes, size exact, one try_match attempt.
 *
 * PROMOTION NOTE: trymatch reports `relocs: name different symbols that resolve
 * to the same address` at +0x37c -- the documented false mismatch -- and this
 * function's data/promoted.json entry needs
 *     "rodata": ["0x0812A14C"]
 * before tools/split_rodata.py and tools/gen_lds.py are re-run.
 *
 * The map-screen setup: clears the gUnknown_084C30F8 view state, builds the
 * cursor / terrain / unit tile and palette sets into VRAM, registers the BG
 * layers, and finally re-programs the four BG-control shadows plus the window
 * and blend shadows.
 *
 * 0x0812A14C is agbcc's own -fforce-addr address constant for
 * &gUnknown_084C30F8, so the ROM's `ldr rN,=word; ldr rM,[rN]; ldr/str via rM`
 * chain is exactly what the honest `gUnknown_084C30F8->m` spelling emits.  It
 * is NOT a pointer-to-pointer and no new symbol needs declaring.
 *
 * TWO LOCAL BINDINGS ARE LOAD-BEARING and they are the only thing that took a
 * second probe round:
 *   - `v` for gUnknown_0200C420.unk00.  Written inline twice, the `str` to
 *     unk02c invalidates CSE (the store may alias the global) and agbcc
 *     re-`ldr`s it for unk028, one instruction more than the ROM.
 *   - `p` for gUnknown_084C30F8 across the unk837 / unk836 pair, same reason on
 *     the two byte stores.
 * NOTE THE CONTRAST INSIDE THIS SAME FUNCTION: the unk030 / unk834 / unk83c /
 * unk832 / unk838 block at the top must NOT be bound to a local -- the ROM
 * reloads the pointer for every one of those five stores, which is what the
 * plain `gUnknown_084C30F8->m` spelling gives.  Same global, same kind of
 * store, opposite spelling, twenty instructions apart.  Binding it there costs
 * bytes; not binding it at unk837/unk836 costs bytes.
 *
 * `gUnknown_084C30F8->unk832 = -0x38` emits `ldr r0,=0x0000FFC8` rather than
 * the cheaper `movs #0x38; rsbs`: PROMOTE_MODE holds HImode values
 * zero-extended, so a HImode constant is materialised as `INTVAL & 0xffff` and
 * 0xFFC8 needs a pool word.  Same for the `-0x30` and `-2` halfword stores near
 * the end.  Do not "fix" these into positive hex constants.
 *
 * The `& 0x3ff` tile-index wraps are authored exactly as written; agbcc CSEs
 * the `i * 32` and `i * 8` subexpressions and reuses them across all four
 * CpuFastSet calls, and the second loop reuses the SAME two registers in place
 * (`adds r5,#0x28` on top of `i * 4`) because the first use is dead there.
 *
 * The four BG shadows are `union BgCntBuf` from include/hardware.h and the
 * writes are `.bits.priority = 0/1/2/3`.  The last one is a bare `orrs #3` with
 * no preceding AND because fold reduces `(x & ~3) | 3` to `x | 3` -- that is an
 * all-ones bitfield ASSIGNMENT, not a `|=`.  The three gDispIo window bits are
 * three separate one-bit field clears that combine merges into a single
 * load/store with three ANDs, and gUnknown_030030E0's `& 0x3f` is
 * `union BlendCntBuf`'s two-bit `effect` field. */

void ShopScreen_Init(void)
{
    struct Unk084C30F8 *p;
    u32 v;
    u16 i;

    PlayMusic(0x12e);
    BuildShopItemList(gUnknown_02028E1C);

    gUnknown_084C30F8->unk030 = 0;
    gUnknown_084C30F8->unk834 = 0;
    gUnknown_084C30F8->unk83c = 0;
    gUnknown_084C30F8->unk832 = -0x38;
    gUnknown_084C30F8->unk838 = 0;

    LoadBg1WindowFrame(0);
    ApplyWindowFramePalette(0, 1);
    LoadCursorSpriteGraphics();

    gUnknown_084C30F8->unk020 = 0;
    gUnknown_084C30F8->unk01e = 0;
    v = gUnknown_0200C420.unk00;
    gUnknown_084C30F8->unk02c = v;
    gUnknown_084C30F8->unk028 = v;

    InitTextTileCache(0);

    for (i = 0; gUnknown_02028E1C[i] != 0xff; i++)
        ;

    p = gUnknown_084C30F8;
    p->unk837 = i;
    p->unk836 = i;

    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();

    Decompress(gUnknown_0823A3D4,
               (void *)(0x06000000 + gUnknown_0300251C.bits.chr_block * 0x4000));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    ApplyPaletteExt(gUnknown_0823BE00, 0, 0x20);
    BG_EnableSyncBG3();
    ApplyWindowFramePalette(0, 3);
    Decompress(gUnknown_0823E7D4, gBG2TilemapBuffer);

    for (i = 0; i <= 0x3ff; i++)
        gBG2TilemapBuffer[i] += 0x1360;

    gUnknown_03001400 = -0x30;
    BG_EnableSyncBG2();
    StartHeaderBanner(gUnknown_085802B4, gUnknown_0200FC50, 0, 5, 1, 1, 3);
    LoadCoFace(0xf, (void *)0x06011560, 0x16);
    Decompress(gUnknown_0823EA40, gUnknown_0200FC50);

    for (i = 0; i <= 7; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + ((i * 32) & 0x3ff) * 32,
                   (void *)(0x06010000 + ((i * 8 + 0xeb) & 0x3ff) * 32), 0x40);
        CpuFastSet(gUnknown_0200FC50 + ((i * 32 + 8) & 0x3ff) * 32,
                   (void *)(0x06010000 + ((i * 8 + 0x12b) & 0x3ff) * 32), 0x40);
        CpuFastSet(gUnknown_0200FC50 + ((i * 32 + 0x100) & 0x3ff) * 32,
                   (void *)(0x06010000 + ((i * 8 + 0x16b) & 0x3ff) * 32), 0x40);
        CpuFastSet(gUnknown_0200FC50 + ((i * 32 + 0x108) & 0x3ff) * 32,
                   (void *)(0x06010000 + ((i * 8 + 0x1ab) & 0x3ff) * 32), 0x40);
    }

    ApplyPaletteExt(gUnknown_0823FB7C[GetLoadedCoPalette(0xf)], 0x2c0, 0x20);

    gUnknown_084C30F8->unk835 = 1;

    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    sub_0801F114();
    InitTilePool(0,
                 (void *)(0x06000000 + gUnknown_03002B6C.bits.chr_block * 0x4000),
                 0x180, 9);
    LoadTilePoolGraphic(0x18);
    LoadTilePoolGraphic(0x19);
    LoadTilePoolGraphic(0x1a);
    LoadTilePoolGraphic(0x13);
    LoadTilePoolGraphic(0x14);
    InitTilePool(2, (void *)0x06010000, 0x7f, 0x14);
    LoadTilePoolGraphic(0x43);
    LoadTilePoolGraphic(0x44);
    LoadTilePoolGraphic(0x51);
    Decompress(gUnknown_0823E8E8, gUnknown_0200FC50);
    CpuFastSet(gUnknown_0200FC50, (void *)0x06011360, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x400, (void *)0x060113E0, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x80, (void *)0x06011460, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x480, (void *)0x060114E0, 0x20);
    ApplyWindowFramePalette(0, 0x18);
    Decompress(gUnknown_0823E140, gUnknown_0200FC50);

    for (i = 0; i <= 1; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (i * 4 + 8) * 32,
                   (void *)(0x06010000 + (i * 8 + 0x5d) * 32), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (i * 4 + 0x28) * 32,
                   (void *)(0x06010000 + (i * 8 + 0x61) * 32), 0x20);
    }

    CpuFastSet(gUnknown_0200FE50, (void *)0x06010DA0, 0x10);
    CpuFastSet(gUnknown_0200FE50 + 0x400, (void *)0x06010DE0, 0x10);
    ApplyPaletteExt(gUnknown_0823E550, 0x2e0, 0x20);

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    gDispIo.disp_ct.win0_enable = 0;
    gDispIo.disp_ct.win1_enable = 0;
    gDispIo.disp_ct.objwin_enable = 0;

    gUnknown_030030E0.bits.effect = 0;

    gUnknown_03001418 = 0;
    gUnknown_03001FF8 = 0;
    gUnknown_03002F18 = -2;
    gUnknown_03002B34 = 0;
}

asm(".global sub_080489CC\n.thumb_set sub_080489CC, ShopScreen_Init\n");

extern bool8 IsMusicFadeActive(void);
extern void StartShopDrawScript(void);
extern void ShopScreen_AddScrollBackgroundHook(void);
extern void ShopScreen_PickGreeting(void);
extern void ShopScreen_WaitGreetingThenSlide(void);
extern void ShopList_ResetRise(void);
extern void ShopList_RiseStep(void);
extern void ShopScreen_Loop(void);
extern void ShopScreen_WaitMessageEnd(void);
extern void ShopScreen_UpdateMainMenuLock(void);
extern u8 GetMainMenuLock(void);
extern void ClearVBlankHooks(void);
extern void ShopScreen_EndMessageScripts(void);
extern void StartProfileSaveScreen(void);
extern void ReturnToMainMenu(void);

struct ProcCmd CONST_DATA ProcScr_BattleMaps[] =
{
    PROC_29(1),
    PROC_1D(30),
    PROC_WHILE(IsMusicFadeActive),
    PROC_START_CHILD(ProcScr_DialogueOnEnd),
    PROC_CALL(ShopScreen_Init),
    PROC_CALL(StartShopDrawScript),
    PROC_CALL(ShopScreen_AddScrollBackgroundHook),
    PROC_1E(30),
    PROC_SLEEP(10),
    PROC_REPEAT(ShopScreen_PickGreeting),
    PROC_REPEAT(ShopScreen_WaitGreetingThenSlide),
    PROC_END_EACH(ProcScr_DialogueOnEnd),
    PROC_CALL(ShopList_ResetRise),
    PROC_REPEAT(ShopList_RiseStep),
    PROC_REPEAT(ShopScreen_Loop),
    PROC_REPEAT(ShopScreen_WaitMessageEnd),
    PROC_CALL(ShopScreen_UpdateMainMenuLock),
    PROC_GOTO_IF_NO(GetMainMenuLock, 0),
    PROC_1D(30),
    PROC_CALL(ClearVBlankHooks),
    PROC_CALL(ShopScreen_EndMessageScripts),
    PROC_CALL(StartProfileSaveScreen),
    PROC_YIELD,
    PROC_GOTO(1),
PROC_LABEL(0),
    PROC_SLEEP(30),
    PROC_29(0),
    PROC_1D(30),
    PROC_CALL(ClearVBlankHooks),
    PROC_CALL(ShopScreen_EndMessageScripts),
PROC_LABEL(1),
    PROC_CALL(ReturnToMainMenu),
    PROC_END,
};

asm(".global gUnknown_084C3138\n.set gUnknown_084C3138, ProcScr_BattleMaps\n");
