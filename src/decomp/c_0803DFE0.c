#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DFE0.
 * sub_0803DFE0 @ 0x0803DFE0
 */

/* Decodes one entry's tile position. The `ldrh; lsls #0x16; lsrs #0x1c` is a
 * BITFIELD read of unk02's bits 6..9, not `(unk02 >> 6) & 0xf` -- the mask
 * spelling costs a `movs #0xf` and an extra `and`.
 *
 * A `switch` (repeated `cmp r2,#3` with `beq` then `bgt`), and the case-block
 * ORDER matters exactly as it does in src/decomp/c_0803DF98.c: gcc emits the
 * blocks in source order and cross-joins their shared `strh [r1,#2];
 * movs r0,#1` tail, so the {1,4} arm has to come first. */
bool8 GetInventionTargetCell(struct Unk02028360 *p, struct Unk02028360Pos *out)
{
    switch (p->unk02_6) {
    case 1:
    case 4:
        out->unk00 = p->unk00;
        out->unk02 = p->unk01;
        return TRUE;
    case 3:
    case 5:
        out->unk00 = p->unk00 + 1;
        out->unk02 = p->unk01 + 2;
        return TRUE;
    }
    return FALSE;
}
asm(".global sub_0803DFE0\n.thumb_set sub_0803DFE0, GetInventionTargetCell\n");
