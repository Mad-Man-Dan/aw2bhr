#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075BF4.
 * sub_08075BF4 @ 0x08075BF4
 */

#include "proc.h"
/* The c_08075C98.c on-screen guard over a AP_Update blit, then the
 * c_08075DBC.c palette/flush tail behind the flag ColorWorldMapSectionWithBank returned.
 *
 * The 0xFF that the ROM parks in the callee-saved r6 is ONE constant with TWO
 * uses: `proc->unk64 & 0xFF` and `proc->unk30 & 0xFF`. The first folds into a
 * `ldrb` because the result goes straight back out through a `strh` and the
 * high byte cannot survive the truncation, which is why that mask is invisible
 * in the instruction stream -- but it is what creates the pseudo early enough
 * to win a callee-saved register, and spelling it `*(u8 *)&proc->unk64` emits
 * the same `ldrb` while leaving the later mask in a scratch. */
struct Unk8075BF4
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x18];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u8 filler_4e[0x6];
    /* 0x54 */ struct Unk0801C210 *unk54;
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x4];
    /* 0x64 */ s16 unk64;
};

void WorldMapMissionClear_Init(struct Unk8075BF4 *proc)
{
    proc->unk5c = ColorWorldMapSectionWithBank(proc->unk58, 0xF, (u8 *)&proc->unk64);
    proc->unk64 = proc->unk64 & 0xFF;
    proc->unk4c = 0;

    if ((u32)(proc->unk2c + 0x10) <= 0x100
        && proc->unk30 >= -0x10
        && proc->unk30 <= 0xa0)
        AP_Update(proc->unk54, proc->unk2c & 0x1ff, proc->unk30 & 0xFF);

    if (proc->unk5c != 0)
    {
        RegisterDataMove(gUnknown_08614280, (void *)0x0600F000, 0x1000);
        CopyToPaletteBufferNoSync(gUnknown_081D1504 + (proc->unk64 - 1) * 0x10, 0x1E0, 0x20);
        ColorFadeInit();
        LoadFadeComponentsFromPalette(0xF, 1);
        EnablePaletteSync();
    }
}
asm(".global sub_08075BF4\n.thumb_set sub_08075BF4, WorldMapMissionClear_Init\n");
