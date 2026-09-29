#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072A88.
 * sub_08072A88 @ 0x08072A88
 */

#include "hardware.h"

/* One `if` per BG-control shadow, in BG0..BG3 order, and all four bodies are
 * the same statement: when the shadow is in 16-colour mode, blank the first
 * 0x10 halfwords of that BG's character block.
 *
 * The two reads of the same shadow use two different access widths and that is
 * get_best_mode, not an inconsistency: `color_depth` is one bit inside byte 0
 * and its `== 0` test is a bare mask, so it comes out `ldrb` + `and #0x80`,
 * while `chr_block` is extracted by shifting and takes the whole aligned word
 * (`ldr`, `lsl #0x1c`, `lsr #0x1e`). Both are the same union.
 *
 * `chr_block * 0x4000` and not `chr_block << 14`: the multiply is what
 * shorten_binary_op folds into the single `lsl #0xe` here. */

void Clear4bppFirstTm(void)
{
    if (gUnknown_03002B6C.bits.color_depth == 0)
        sub_08001148((u16 *)(gUnknown_03002B6C.bits.chr_block * 0x4000 + 0x06000000), 0x10, 0);

    if (gUnknown_03001FE8.bits.color_depth == 0)
        sub_08001148((u16 *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06000000), 0x10, 0);

    if (gUnknown_030030B4.bits.color_depth == 0)
        sub_08001148((u16 *)(gUnknown_030030B4.bits.chr_block * 0x4000 + 0x06000000), 0x10, 0);

    if (gUnknown_0300251C.bits.color_depth == 0)
        sub_08001148((u16 *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06000000), 0x10, 0);
}
asm(".global sub_08072A88\n.thumb_set sub_08072A88, Clear4bppFirstTm\n");
