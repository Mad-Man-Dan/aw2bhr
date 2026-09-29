#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806BB08.
 * CreditsStaff_Init @ 0x0806BB08
 */

#include "hardware.h"
#include "proc.h"
struct Unk0806BB08Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
};

/* MATCH.
 *
 * The wave-18 residual (94.2%, size-exact, 31 bytes) was diagnosed as a shared
 * zero hoisted into a callee-saved register, and it was not: it was the blend
 * read-modify-write, spelled through a `u16 v` temp. Two store statements
 * through the scalar cast -- the MapSelect_SetBlend spelling -- fix the whole 31
 * bytes at once, including the `movs r0, #0` the ROM re-materialises for
 * `proc->unk38 = 0` after the three calls, and including the r0/r1 roles at
 * that store. SoundRoomMusicPage_Init had the identical residual from the identical
 * spelling. Nothing about the zero needed changing. */
void CreditsStaff_Init(struct Unk0806BB08Proc *proc)
{
    SetDefaultColorEffects();
    Proc_EndEach(gUnknown_0858175C);
    ApplyBgControlTable(gUnknown_085819E4);
    SetDispEnable(1, 1, 1, 1, 1);
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    FillHalfwordsUnsigned(gBG0TilemapBuffer, 0x400, 0);
    FillHalfwordsUnsigned(gBG1TilemapBuffer, 0x400, 0);
    FillHalfwordsUnsigned(gBG2TilemapBuffer, 0x400, 0);
    ApplyPalette(gUnknown_081951F4, 15);
    ApplyPalette(gUnknown_08195214, 0);
    ApplyPalette(gUnknown_08195214, 16);
    Decompress(gUnknown_08194614, (void *)0x06000000);
    Decompress(gUnknown_08191ADC, (void *)0x06001400);
    Decompress(gUnknown_081B9A38, (void *)0x06001C00);
    Decompress(gUnknown_08194A9C, (void *)0x06008000);
    Decompress(gUnknown_0819507C, gBG3TilemapBuffer);
    Decompress(gUnknown_08191ADC, (void *)0x06011400);
    sub_080718F8(gBG0TilemapBuffer + 1, gUnknown_08195234, 0);
    sub_080718F8(gBG1TilemapBuffer, gUnknown_081952D4, 0);
    sub_080718F8(gBG2TilemapBuffer + 7, gUnknown_081952D4, 0);
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    SetBgScrollShadow(0, 0, 0xFFFC);
    Proc_Start(gUnknown_085819D4, proc);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 9;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x07;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x1800;
    SetCreditsWindow0Rect(0, 0, 0x40, 0xa0);
    SetSoundMixerChannelCount(0xa);
    PlayMusic(0x1a0);
    proc->unk38 = 0;
}
asm(".global sub_0806BB08\n.thumb_set sub_0806BB08, CreditsStaff_Init\n");
