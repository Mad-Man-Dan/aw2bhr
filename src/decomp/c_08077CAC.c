#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077CAC.
 * sub_08077CAC @ 0x08077CAC
 */

/* WAVE 76: MATCHED. The final zero-placement residual requires making the
 * post-WorldMapMissionInfo_PutSprites zero an explicit r4 inline-asm output, immediately copying
 * it to a fixed r8 local for the later proc->unk44 store, and separately
 * storing it to the stack cell passed to CpuFastSet. This reproduces
 * `movs r4,#0; mov r8,r4; str r4,[sp,#4]` without letting GCSE hoist the zero
 * above the call. The fixed r4 temporary must remain block-scoped; extending
 * its lifetime swaps the surrounding callee-saved allocation.
 *
 * WAVE 72 (W72-F): configured baseline reverified unchanged at exact size
 * 324/324, 92.0%, 26 differing bytes. Per the batch brief this already-ground
 * order case was not rewritten: the same two pool-register/copy-placement
 * residuals remain and no semantic or size axis reopened.
 *
 * PARKED at 92.0%, SIZE EXACT (324/324), 26 bytes differing and every one of
 * them a REGISTER NUMBER. The statement structure, the constants, the branch
 * senses and all thirteen calls are byte-identical.
 *
 * Two residuals, and they are probably one fact:
 *   1. The two pool addresses swap hard registers -- the ROM puts
 *      &gUnknown_030030E0 (3 references) in r2 and &gUnknown_03001FFC (1) in
 *      r3, this candidate the other way round.
 *   2. `movs r4, #0; mov r8, r4` (the shared zero: CpuFastSet's source local
 *      and `proc->unk44 = 0`) lands BEFORE the `strh` that ends the
 *      gUnknown_030030E0 raw write here, and AFTER `bl WorldMapMissionInfo_PutSprites` in the
 *      ROM.
 *
 * MEASURED and worth keeping whatever happens to the rest:
 *   - agbcc for Thumb does NOT schedule across statements. Moving the
 *     `gUnknown_030030E0.raw` write up two statements moved its instructions
 *     exactly, dropping the score to 26.9% -- so the emitted order IS the
 *     source order and the order above is settled, not a guess. (It also
 *     FIXED residual 1: with the two 030030E0 references adjacent the ROM's r2
 *     allocation appears. The register swap is therefore a live-range effect
 *     of the three volatile stores sitting between them, not a spelling.)
 *   - The five target1 bits are NOT five `.bits` assignments. Five adjacent
 *     one-bit fields merge into ONE byte-wide read-modify-write (c_080039E4.c
 *     records that), and the ROM's access is `ldrh; and 0xFFE0; orr 0x1D;
 *     strh` -- a halfword, so it is a `.raw` write. `effect = 3` immediately
 *     before it stays `ldrb; orr 0xC0; strb` because a 2-bit field set to all
 *     ones drops the AND and cannot merge with a differently-sized access.
 *   - `m = proc->unk4c;` as its own statement before `n = 0x1E - t;` is worth
 *     14 bytes: without it agbcc emits the subtraction first and the `ldrsh`
 *     is then forced into r0 instead of r1.
 *   - StartWorldMapSelectionFrame is declared `(s16, s16, int, ProcPtr)`, so the two
 *     `lsls #0x10; asrs #0x10` pairs come free from the prototype; no explicit
 *     cast is needed and adding one changes nothing.
 *
 * Per-frame handler for the closing wipe: interpolate the wipe width, clamp it
 * to +0x4c, set up the blend and the scroll, blit one column band, and on the
 * sixth frame tear the whole thing down and hand the map position to
 * StartWorldMapSelectionFrame before breaking. */
#include "global.h"
#include "proc.h"
#include "hardware.h"
struct Unk8077CAC
{
    /* 0x00 */ u8 filler_00[0x44];
    /* 0x44 */ int unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4a */ s16 unk4a;
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
};

void WorldMapMapPreview_CloseLoop(struct Unk8077CAC *proc)
{
    int t;
    int n;
    int m;
    register u32 zero_saved asm("r8");
    u32 zero_cell;

    t = Interpolate(0, proc->unk48, 0x1E, proc->unk44, 5);

    m = proc->unk4c;
    n = 0x1E - t;
    if (n > m)
        n = m;

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 5 - proc->unk44;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x1D;

    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);

    {
        register u32 zero asm("r4");

        asm("movs %0, #0" : "=r" (zero));
        zero_saved = zero;
        zero_cell = zero;
    }
    CpuFastSet(&zero_cell, gBG1TilemapBuffer, 0x01000140);

    sub_08071900(gUnknown_08551A04 + 0x100,
                 gBG1TilemapBuffer + (proc->unk4a * 32 + t), n, proc->unk4e);

    BG_EnableSyncBG1();

    if (proc->unk44 > 4)
    {
        proc->unk44 = zero_saved;
        EndMapPreviewEffects();
        SetDefaultColorEffects();
        SetDifficultyStarsPalette(2);
        SetWorldMapScopePalette(2);
        ClearBg1Tilemap();
        HideMapPreview();
        StartWorldMapSelectionFrame(gUnknown_08615194[gUnknown_0202FDFC.unk0c].flagX
                         - gUnknown_0202FDFC.unk00 + 1,
                     gUnknown_08615194[gUnknown_0202FDFC.unk0c].flagY
                         - gUnknown_0202FDFC.unk02 + 2,
                     2, proc);
        Proc_Break(proc);
    }
    else
    {
        proc->unk44++;
    }
}
asm(".global sub_08077CAC\n.thumb_set sub_08077CAC, WorldMapMapPreview_CloseLoop\n");
