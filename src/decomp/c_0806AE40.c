#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AE40.
 * sub_0806AE40 @ 0x0806AE40
 */

/* One row of gUnknown_0858178C loaded into a proc-supplied buffer, then that
 * buffer biased by a per-proc palette index.
 *
 * The `proc->unk2c == 7` arm shows a bare `+0x9c` (== 7 * 0x14 + 0x10) with no
 * index scaling for exactly the reason c_0806AEC4.c records: cse's
 * record_jump_equiv knows the value is 7 inside the taken arm and folds the
 * index, so both arms are the same expression in the source.
 *
 * `proc->unk38` is re-read on EVERY iteration of the bias loop rather than
 * hoisted, and that is not a volatile tell: the loop stores through
 * `proc->unk34`, which may alias the proc itself, so the invariant cannot be
 * proved. The destination pointer IS bound to a local -- loop.c will not hoist
 * that load either, so the ROM's single `ldr r4, [r5, #0x34]` in the preheader
 * has to come from the source. */

struct Unk806AE40
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x04];
    /* 0x34 */ u16 *unk34;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
};

void CreditsIllustration_LoadTilemapAndPalette(struct Unk806AE40 *proc)
{
    u16 *dst;
    int i;

    Decompress(gUnknown_0858178C[proc->unk2c].unk0c, proc->unk34);

    if (proc->unk2c == 7)
        ApplyPaletteExt(gUnknown_0858178C[proc->unk2c].unk10, proc->unk38 << 5, 0xe0);
    else
        ApplyPaletteExt(gUnknown_0858178C[proc->unk2c].unk10, proc->unk38 << 5, 0xc0);

    dst = proc->unk34;
    for (i = 0; i < 0x280; i++)
        dst[i] += proc->unk38 << 12;

    sub_08013AD4(proc->unk39);
}
asm(".global sub_0806AE40\n.thumb_set sub_0806AE40, CreditsIllustration_LoadTilemapAndPalette\n");
