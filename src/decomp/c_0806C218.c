#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C218.
 * sub_0806C218 @ 0x0806C218
 */

#include "hardware.h"
#include "proc.h"
/* Picks the row of gUnknown_085816F0 for this run and loads its palette and
 * graphics. `5 - GetAverageCampaignRank()` stays in r0 across the +0x60 store, so the
 * first table read uses the register while the second (after two calls) has to
 * reload -- that asymmetry is CSE, not two different expressions.
 *
 * The +0x60 value is a CONDITIONAL EXPRESSION and its polarity is a readout:
 * the ROM presets 0x10 and overwrites with 0x20 on the branch NOT taken, i.e.
 * the `!= 3` orientation. Written `== 3 ? 0x10 : 0x20` agbcc presets 0x20 and
 * inverts the branch -- same size, different bytes. */
struct Unk0806C218Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void CreditsRank_Init(struct Unk0806C218Proc *proc)
{
    proc->unk58 = 5 - GetAverageCampaignRank();
    proc->unk60 = (proc->unk58 != 3) ? 0x20 : 0x10;

    ApplyPaletteExt(gUnknown_085816F0[proc->unk58].unk04, 0x220, 0x20);
    ApplyPaletteExt(gUnknown_081A3D24, 0x240, 0x20);
    Decompress(gUnknown_085816F0[proc->unk58].unk00, (void *)0x06011200);
    Decompress(gUnknown_081A3BD4, (void *)0x06011A00);

    gUnknown_0202F2C0 = 0;
    proc->unk5c = 0;
}
asm(".global sub_0806C218\n.thumb_set sub_0806C218, CreditsRank_Init\n");
