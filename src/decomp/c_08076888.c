#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076888.
 * sub_08076888 @ 0x08076888
 */

#include "proc.h"
#include "hardware.h"
/* WAVE 72 (W72-F): MATCHED configured at 480/480. A fresh scalar pseudo for
 * the terminal 0xFFFF value is the missing lowering lever: `end = 0xFFFF;
 * *list = end;` emits the ROM's direct `ldr r0; strh r0, [r7]`, while the
 * immediate store copies the pool-loaded value through r1 and costs 4 bytes.
 * The match retains the honest &gUnknown_0202FDFC.unk3c spelling; its symbol
 * plus addend resolves to the ROM's invented gUnknown_0202FE38 name. The two
 * force-address words at 0x081CC588/58C still require normal rodata placement
 * at promotion. */


/* WAVE 66 (W66-I): the old draft no longer compiled after wave 38 correctly
 * removed the fictional standalone `u16 gUnknown_0202FE38`. Binding the real
 * address, `&gUnknown_0202FDFC.unk3c`, at entry restores the ROM's long-lived
 * r7 and raises the configured draft to 88.5%, but it is genuinely 4 bytes
 * long (484/480): an indirect u16 store emits `ldr r1, =0xFFFF; adds r0, r1,
 * #0; strh r0, [r7]`, where the ROM has `ldr r0; strh r0, [r7]`. u16/s16,
 * -1/0xFFFF, volatile, and direct-address pointer probes do not remove that
 * copy without losing the early r7 address. The source/type model is now
 * correct; the remaining axis is direct-global versus pointer-store lowering.
 *
 * HISTORICAL PARK. SIZE WAS EXACT (480/480) and the whole residual was ONE extra
 * simultaneously-live value: the first difference is at +0x0, which is the
 * PUSH LIST -- the ROM pushes {r4, r5, r6, r7, lr} and this candidate
 * {r4, r5, r6, lr}. Do not read the 24.6% as a body problem; the percentage
 * counts register fields and every register above r3 is shifted by the missing
 * r7. The earlier 30.4% draft scored HIGHER with the if/else arms the wrong
 * way round, so the score is not the ranking here.
 *
 * What r7 holds in the ROM: `ldr r7, =gUnknown_0202FE38` is the FIRST
 * instruction after the prologue, and its only use is the `strh` of 0xFFFF
 * five statements from the end. This candidate materialises that address at
 * the store instead and spends `ldr r1; ldr r2; adds r0, r2, #0; strh r0,[r1]`
 * where the ROM spends `ldr r0; strh r0,[r7]` -- +4 bytes there, -2 for the
 * missing `ldr r7` at the top and -2 for the `.short 0x0000` it pushes the
 * function over into, which is why the total still lands on 480.
 *
 * RULED OUT: the scheduler. agbcc for Thumb does not move instructions across
 * statements (measured on WorldMapMapPreview_CloseLoop this wave -- moving one statement moved
 * its instructions exactly), so the `ldr r7` at the top is NOT a sunk store or
 * a hoisted load; the SET really is generated there. No placement of
 * `gUnknown_0202FE38 = 0xFFFF;` in this statement order can produce it, and
 * moving the statement itself moves the `strh` with it.
 *
 * SETTLED, and all of it converted:
 *   - `if (IsHardCampaignMode() == 0)` with the gUnknown_081D208C arm as the
 *     FALLTHROUGH. agbcc lays the then-arm out as the fallthrough and the
 *     else-arm at the branch target, so `if (IsHardCampaignMode())` with the arms
 *     the natural way round is backwards. The 208C arm also re-uses r4 from
 *     the ApplyPaletteExt two statements earlier, which is what identifies it.
 *   - gUnknown_081CC588 and gUnknown_081CC58C are FORCE-ADDR POOL WORDS, not
 *     objects: they hold 0x03002B6C and 0x030030B4, and naming
 *     gUnknown_03002B6C / gUnknown_030030B4 directly reproduces both
 *     `ldr rN, =.LCn; ldr rM, [rN]` pairs. This continues the run W35-B
 *     audited at 0x081CC590/594/598/59C and 0x081CC584 (WorldMapNationPanel_Refresh, matched
 *     this wave). Each is referenced TWICE, which is the trigger.
 *   - `.bits.priority = 3` emits a bare `orrs #3` with no AND: a bitfield set
 *     to all ones drops the mask (store_fixed_bit_field's all_one case), the
 *     same fold c_080039E4.c records for `effect = 3`.
 *   - gSmoothScroll's five zero stores really are in ROM order
 *     frameCounter, currentY, currentX, targetY, targetX -- reverse
 *     declaration order.
 *   - StartWorldMapMarkerDrawer takes a ProcPtr (promoted in c_08074714.c) and is called
 *     with a literal 4, hence the cast. */
void SetupWorldMapScreen(ProcPtr proc)
{
    u16 *list;
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 end;

    list = (u16 *)&gUnknown_0202FDFC.unk3c;

    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
    SetupBackgrounds(gUnknown_08614548);

    gDispIo.disp_ct.bg0_enable = 1;
    gDispIo.disp_ct.bg1_enable = 1;
    gDispIo.disp_ct.bg2_enable = 1;
    gDispIo.disp_ct.bg3_enable = 1;
    gDispIo.disp_ct.obj_enable = 1;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    a = 0;
    CpuFastSet(&a, gBG0TilemapBuffer, 0x01000200);
    b = 0;
    CpuFastSet(&b, gBG1TilemapBuffer, 0x01000200);
    c = 0;
    CpuFastSet(&c, gBG2TilemapBuffer, 0x01000200);

    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();

    Decompress(gUnknown_081CC5F0, (void *)0x06008000);
    ApplyPaletteExt(gUnknown_081D1504, 0xC0, 0x120);
    Decompress(gUnknown_081D1644, (void *)0x06010900);
    Decompress(gUnknown_081D17A4, (void *)0x06010B00);
    Decompress(gUnknown_081D2A54, (void *)0x06010000);
    ApplyPaletteExt(gUnknown_081D208C, 0x200, 0x20);

    if (IsHardCampaignMode() == 0)
        ApplyPaletteExt(gUnknown_081D208C, 0x220, 0x20);
    else
        ApplyPaletteExt(gUnknown_081D20CC, 0x220, 0x20);

    LoadWindowFrameGraphics((void *)(gUnknown_030030B4.bits.chr_block * 0x4000 + 0x06006C00),
                 0, 0);

    d = 0;
    CpuFastSet(&d,
               (void *)(gUnknown_03002B6C.bits.chr_block * 0x4000 + 0x06000000),
               0x01000008);

    SetBgScrollShadow(3, gUnknown_0202FDFC.unk00, gUnknown_0202FDFC.unk02);

    end = 0xFFFF;
    *list = end;

    gSmoothScroll.frameCounter = 0;
    gSmoothScroll.currentY = 0;
    gSmoothScroll.currentX = 0;
    gSmoothScroll.targetY = 0;
    gSmoothScroll.targetX = 0;

    StartWorldMapMarkerDrawer((ProcPtr)4);
    ResetWindowShadows();
    SetDefaultColorEffects();
}
asm(".global sub_08076888\n.thumb_set sub_08076888, SetupWorldMapScreen\n");
