#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806938C.
 * sub_0806938C @ 0x0806938C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk6938CProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

/* Wave 53, W53-D. MATCHED first attempt; the whole function reads off
 * c_080691BC.c / c_080694EC.c, which are the same subsystem's already-promoted
 * neighbours and carry the identical `for (i = 0; i < 0x400; i++)
 * gBG0TilemapBuffer[i] += 0x140;` loop and the same 10-argument StartIntroBgAffineTween
 * call.
 *
 * gBG0TilemapBuffer and gBG2TilemapBuffer are REAL ROM pointer variables, not
 * -fforce-addr pool words: the ROM words at 0x08499578 / 0x08499580 hold two
 * different VRAM-bound addresses and both are already declared `u16 *`. The
 * single `ldr =sym; ldr [sym]` indirection is the honest spelling. (Contrast
 * gUnknown_0816E170 in c_0806B1A8.c and gUnknown_081D9450 in c_08087C94.c,
 * which ARE pool words -- checked by dereferencing baserom.gba.)
 *
 * `gUnknown_0202F204++` supplies the OLD value; the `lsls #0x18; lsrs #0x18`
 * pair is agbcc's u8 re-truncation of it, not a separate cast. */
void IntroT3_0806938D(struct Unk6938CProc *proc)
{
    int i;

    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);

    SetDispEnable(1, 1, 0, 0, 1);

    gUnknown_030030B4.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    SetDefaultColorEffects();
    ResetBgAffineToScreenCentre();
    SetBgCntScreenSize((struct Unk8012C30 *)&gUnknown_030030B4, 2);

    Decompress(gUnknown_08183B14, gBG2TilemapBuffer);
    ApplyPaletteExt(gUnknown_08183C28, 0, 0x80);
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
    Decompress(gUnknown_0823A3D4, (void *)0x06002800);
    Decompress(gUnknown_08239FA4, gBG0TilemapBuffer);

    for (i = 0; i < 0x400; i++)
        gBG0TilemapBuffer[i] += 0x140;

    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();

    SetBgScrollShadow(0, 0, 0);
    SetBgScrollShadow(1, 0, 0);
    SetBgScrollShadow(2, 0, 0);

    StartIntroBgScroll(0, 0, -5, proc);
    StartIntroBgAffineTween(2, 1, 0, 0x88, 0x3800, 0, 0xc0, 0x100, 0xc, proc);
}

asm(".global sub_0806938C\n.thumb_set sub_0806938C, IntroT3_0806938D\n");
