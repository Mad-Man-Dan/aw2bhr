#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806EB5C.
 * sub_0806EB5C @ 0x0806EB5C, sub_0806ED7C @ 0x0806ED7C
 */

#include "hardware.h"
#include "proc.h"
struct Unk0806EB5CProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u8 filler_36[0x02];
    /* 0x38 */ u8 unk38;
};
struct Unk0806ED7CProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
};

/* MATCH.
 *
 * The whole residual of the wave-18 draft (93.0%, size-exact) was the blend
 * read-modify-write, and it presented as a WHOLE-FUNCTION register swap: `proc`
 * held r5 and the run of short-lived constants held r4, where the ROM has them
 * the other way round, plus one `movs #0` five instructions early. The wave-18
 * draft wrote the pair through a `u16 v` temp:
 *
 *     v = (gUnknown_030030E0.raw & 0xFFE0) | 2;
 *     v = (v & 0xE0FF) | 0x800;
 *     gUnknown_030030E0.raw = v;
 *
 * That temp is one pseudo spanning both statements, and it is what pushes the
 * parameter out of r4. Written as two store statements through the scalar cast
 * -- the MapSelect_SetBlend spelling -- the parameter keeps r4, the second zero's
 * `movs` lands back inside `proc->unk38 = 0`, and every byte falls out. See the
 * mask-and-set table under Bitfields: `.raw` here also ORs in the live zero
 * from `gUnknown_03001FFC = 0` as a spurious `orrs r0, r3`.
 *
 * A `do { } while (0)` round the pair was tried and is NOT needed -- measured
 * both ways, byte-identical. This was not an allocation tie-break.
 */
void SoundRoomMusicPage_Init(struct Unk0806EB5CProc *proc)
{
    ResetWindowShadows();
    SetDefaultColorEffects();
    SetupBackgrounds(gUnknown_0849D16C);
    gDispIo.disp_ct.obj_mapping = 1;
    SetDispEnable(1, 1, 1, 1, 1);
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    ApplyWindowFramePalette(0, 3);
    Decompress(gUnknown_0823A3D4, (void *)0x06008000);
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    Decompress(gUnknown_081A3E28, (void *)0x06000400);
    sub_080718F8(gBG1TilemapBuffer, gUnknown_081A3E3C, 0x1020);
    Decompress(gUnknown_081A4000, (void *)0x06010000);
    Decompress(gUnknown_081A4450, (void *)0x06016000);
    LoadBg1WindowFrame(0);
    DrawWindowBackgroundOnBg2(6, 0xf, 0x12, 4);
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x02;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x800;
    proc->unk30 = 1;
    gUnknown_0202F2D8 = 0;
    proc->unk38 = 0;
    SoundRoomDrawTrackTitle(proc->unk30, 0x40);
    SetBgScrollShadow(0, 0xFFC8, 0);
    proc->unk2c = 0;
    proc->unk34 = gUnknown_08582764[proc->unk30].unk04;
    SoundRoomCycleBgPalette(proc);
    StartHeaderBanner(gUnknown_085826E0, gUnknown_02010C50, 0x50, 0xe, 0xa, 0, (int)proc);
    StartSoundRoomCoPortrait(proc->unk34, proc);
    StartSoundRoomArrows(0, proc);
    SpawnSoundRoomButtons(proc);
    StartSoundRoomLevelPulse(proc);
    StartBgScrollAnimator(8, gUnknown_08582C7C, proc);
    Proc_Start(gUnknown_08614200, 0);
}
asm(".global sub_0806EB5C\n.thumb_set sub_0806EB5C, SoundRoomMusicPage_Init\n");

/* The resume twin of SoundRoomMusicPage_Init -- the first 27 statements are identical, and
 * the whole of this function was written by copying that one and substituting
 * the tail. Three things differ:
 *
 *   * a window-enable block on the two WinCnt shadows, which is why the frame
 *     pushes three high registers where EB5C pushes one: the constants 2, 4, 8,
 *     0x10 and 0x40 that SetDispEnable and obj_mapping materialise at the top
 *     are all reused here, so five of them stay live across the whole body;
 *   * the four proc fields come out of the 0202F2CC run instead of being
 *     hard-coded, and there is no 0202F2D8 store and no unk38;
 *   * the trailing Proc_Start is guarded on proc->unk2c.
 *
 * The A4 store sinks past the DC store because its byte-1 group is written on
 * both sides of the DC group and agbcc merges the two into one
 * read-modify-write -- see the flush-routine caveat in docs/agbcc-codegen.md.
 */
void SoundRoomMusicPage_Resume(struct Unk0806ED7CProc *proc)
{
    ResetWindowShadows();
    SetDefaultColorEffects();
    SetupBackgrounds(gUnknown_0849D16C);
    gDispIo.disp_ct.obj_mapping = 1;
    SetDispEnable(1, 1, 1, 1, 1);
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    ApplyWindowFramePalette(0, 3);
    Decompress(gUnknown_0823A3D4, (void *)0x06008000);
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    Decompress(gUnknown_081A3E28, (void *)0x06000400);
    sub_080718F8(gBG1TilemapBuffer, gUnknown_081A3E3C, 0x1020);
    Decompress(gUnknown_081A4000, (void *)0x06010000);
    Decompress(gUnknown_081A4450, (void *)0x06016000);
    LoadBg1WindowFrame(0);
    DrawWindowBackgroundOnBg2(6, 0xf, 0x12, 4);
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x02;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x800;

    gUnknown_030030A4.bits.win1_enable_bg0 = 0;
    gUnknown_030030A4.bits.win1_enable_bg1 = 0;
    gUnknown_030030A4.bits.win1_enable_bg2 = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 1;
    gUnknown_030030A4.bits.win1_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    proc->unk30 = gUnknown_0202F2D0;
    proc->unk2c = gUnknown_0202F2CC;
    proc->unk34 = gUnknown_0202F2D4;
    proc->unk36 = gUnknown_0202F2D6;
    SoundRoomDrawTrackTitle(proc->unk30, 0x40);
    SetBgScrollShadow(0, 0x48 - ((proc->unk30 & 1) << 7), 0);
    SoundRoomCycleBgPalette(proc);
    StartHeaderBanner(gUnknown_085826E0, gUnknown_02010C50, 0x50, 0xe, 0xa, 0, (int)proc);
    StartSoundRoomCoPortrait(proc->unk34, proc);
    StartSoundRoomArrows(0, proc);
    SpawnSoundRoomButtons(proc);
    StartSoundRoomLevelPulse(proc);
    StartBgScrollAnimator(8, gUnknown_08582C7C, proc);
    if (proc->unk2c != 0)
        Proc_Start(gUnknown_08582C5C, proc);
}
asm(".global sub_0806ED7C\n.thumb_set sub_0806ED7C, SoundRoomMusicPage_Resume\n");
