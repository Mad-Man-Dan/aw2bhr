#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080763F4.
 * sub_080763F4 @ 0x080763F4
 */

#include "hardware.h"
/* Loads the per-variant tile blob for the overlay and arms the blend registers.
 * The variant index at +0x58 selects an 8-byte record: its first word is the
 * blob for Decompress, its second is copied to +0x60, which c_0807662C.c and
 * WorldMapCallout_HoldLoop both read as the x-offset of the second sprite. The ROM reaches
 * the second word by bumping the record base (`adds r4, #4`) instead of loading
 * a second address, which is what a struct-array subscript gives and two
 * parallel tables would not.
 *
 * The tail is WorldMapReticle_Init's minus the target1_enable_bd clear -- the `& 0xFFE0`
 * with no OR, then the `& 0xE0FF | 0xF00`, both folded into one `ldrh`/`strh`
 * pair, and target2_enable_bd set as a single bit on byte 1. */
struct Unk80763F4
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ u8 filler_5c[0x4];
    /* 0x60 */ int unk60;
};

void WorldMapCallout_Init(struct Unk80763F4 *proc)
{
    ApplyPaletteExt(gUnknown_081D2284, 0x2A0, 0x20);

    Decompress(gUnknown_086144DC[proc->unk58].unk00, (void *)0x06012000);
    Decompress(gUnknown_081D33BC, (void *)0x06012600);

    proc->unk60 = gUnknown_086144DC[proc->unk58].unk04;

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = *(u16 *)&gUnknown_030030E0 & 0xFFE0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0xF00;
    gUnknown_030030E0.bits.target2_enable_bd = 1;
}
asm(".global sub_080763F4\n.thumb_set sub_080763F4, WorldMapCallout_Init\n");
