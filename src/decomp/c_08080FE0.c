#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080FE0.
 * sub_08080FE0 @ 0x08080FE0
 */

/* MainMenuCarouselShimmer_Loop @ 0x08080FE0, 128 bytes, THUMB. Matched.
 *
 * Applies one 0x10-byte palette per unit slot 0..5 whose IsMainMenuTileComplete(i) is
 * truthy, then one more at the fixed slot 0x30A, then ticks the animation
 * counter at proc->unk64. The palette picked is always the same one --
 * `((s16)proc->unk64 >> 2) & 0xF` indexes gUnknown_0812598C -- so the loop
 * varies only in the destination.
 *
 * THE DESTINATION MUST BE WRITTEN WITH REAL SHIFTS, not a `(u16)` cast. The
 * ROM's accumulator holds the value PRE-SHIFTED by 16 (`ldr r6, =0x024A0000`,
 * `+= 0x80 << 0xe` per pass, `lsrs r1, r6, #0x10` at the use), which is
 * strength_reduce turning `(0x24a + i * 0x20) << 16` into a giv. `(u16)x`
 * expands to `x & 0xFFFF`, and an AND is not a giv operation, so the cast
 * spelling emits `lsl r1, r4, #0x15; ldr r2, =0x24a0000; add; lsr #0x10`
 * inside the loop instead. See docs/agbcc-codegen.md.
 *
 * `ldrh; lsls #0x10; asrs #0x12` is a u16 member with an explicit `(s16)`
 * cast then `>> 2` -- an s16 member would have folded to `ldrsh; asrs #2`.
 *
 * `proc + 0x64` is computed inside the loop and read after it; that live
 * range is real, and the tail's `ldrh [r5]` is the same object.
 */
struct Unk08080FE0
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
};

void MainMenuCarouselShimmer_Loop(struct Unk08080FE0 *proc)
{
    int i;

    for (i = 0; i <= 5; i++)
    {
        if (IsMainMenuTileComplete(i))
            ApplyPaletteExt(gUnknown_0812598C + (((s16)proc->unk64 >> 2) & 0xf),
                (u32)((0x24a + i * 0x20) << 16) >> 16, 0x10);
    }

    ApplyPaletteExt(gUnknown_0812598C + (((s16)proc->unk64 >> 2) & 0xf), 0x30a, 0x10);
    proc->unk64++;
}
asm(".global sub_08080FE0\n.thumb_set sub_08080FE0, MainMenuCarouselShimmer_Loop\n");
