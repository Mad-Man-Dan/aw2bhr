#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084700.
 * sub_08084700 @ 0x08084700
 */

#include "hardware.h"
/* WAVE 71 (W71-F): MATCHED configured, 260/260 byte-for-byte. Computing the
 * palette value from `i` inside the loop makes LICM/strength reduction place
 * its load after the source loop init and global hoist, matching the ROM.
 *
 * WAVE 66 (W66-J). PARKED size-exact at 96.9% (8/260 bytes differ), improved
 * from 82.3%. Computing the third palette source through an explicit byte
 * offset fixes the `lsls`/pool-load order. Carrying the loop palette number as
 * 0x024A0000 and passing its upper halfword fixes the ROM's high-halfword IV.
 * The sole residual is preheader order: candidate loads 0x024A0000 before the
 * i/proc/global inits; ROM loads it after them. The body and instruction
 * multiset are exact. One uninterrupted 300-second permuter run (15,347
 * iterations) found no candidate above this fixpoint. */

struct Unk08084700
{
    /* 0x00 */ u8 filler_00[0x4a];
    /* 0x4a */ u16 unk4a;
    /* 0x4c */ u8 filler_4c[0x52 - 0x4c];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x64 - 0x54];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 filler_66[0x6a - 0x66];
    /* 0x6a */ s16 unk6a;
};

void MainMenuCarouselWheel_AnimatePalettes(struct Unk08084700 *proc)
{
    int i;
    int offset;
    u16 *palSrc;
    u32 pal;

    if (proc->unk64 >= 0 && proc->unk6a == 0)
    {
        if (IsMainMenuTileComplete(gUnknown_0861696C[DivRem(proc->unk52 + 2, 6)]) != 0)
            ApplyPaletteExt(gUnknown_0812598C + (((s16)proc->unk4a >> 2) & 0xf),
                            0x30A, 0x10);

        ApplyPaletteExt(
            GetMainMenuTileHighlightPalette(gUnknown_0861696C[DivRem(proc->unk52 + 2, 6)])
                + (((s16)proc->unk4a >> 2) & 0xf),
            0x31E, 2);

        if (IsCampaignCompletionFlagSet(0x20) != 0)
        {
            offset = DivRem(Div(proc->unk4a, 2), 0x10);
            offset <<= 1;
            palSrc = (u16 *)((u8 *)gUnknown_0823DE18 + offset);
            ApplyPaletteExt(palSrc, 0x3BE, 2);
        }
    }

    for (i = 0; i <= 5; i++)
    {
        pal = (0x24A + i * 0x20) << 16;
        if (IsMainMenuTileComplete(i) != 0)
            ApplyPaletteExt(gUnknown_0812598C + (((s16)proc->unk4a >> 2) & 0xf),
                            (u16)(pal >> 16), 0x10);

    }

    proc->unk4a++;
}
asm(".global sub_08084700\n.thumb_set sub_08084700, MainMenuCarouselWheel_AnimatePalettes\n");
