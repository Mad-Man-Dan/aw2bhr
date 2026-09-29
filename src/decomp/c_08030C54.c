#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030C54.
 * sub_08030C54 @ 0x08030C54
 */

/* MATCHED (wave 42, W42-K). Needs its .rodata pool word placed:
 *   "rodata": ["0x08090CE8"]
 * -- an agbcc -fforce-addr word holding &gUnknown_0849B018, not a global.
 *
 * The second argument must be written as a MULTIPLY, not a shift: with
 * DrawSpriteNumberFont2's `u16` second parameter, shorten_binary_op folds
 * `(i + 10) * 8` into the two instructions `lsl #0x13; lsr #0x10` the ROM
 * has, where `(i + 10) << 3` needs three. Read that pair as
 * (u32)x << 19 >> 16 -- a net left shift of three under a u16 truncation --
 * and not as a mask plus a shift.
 *
 * `s8 i` rather than int: PROMOTE_MODE keeps the counter zero-extended
 * (`lsr r0`) while every signed use re-extends it (`lsl #0x18; asr #0x18`),
 * which is why both shifts appear at the bottom of the loop off one value.
 *
 * gUnknown_0849B018 is re-dereferenced inside the loop because DrawSpriteNumberFont2
 * could write the pointer variable. r5 stays live afterwards, which is why
 * the unk1b send at the tail reloads only the pointer and not the pool
 * word. unk16 is a member ARRAY at 0x16 -- `adds rB,#0x16; adds rB,rB,rI`
 * on the record base is the member-array hoist, not a displacement. */

void LinkDrawTimeoutCounters(void)
{
    s8 i;

    for (i = 0; i <= 3; i++)
        DrawSpriteNumberFont2(0xa0, (i + 10) * 8, gUnknown_0849B018->unk16[i]);

    DrawSpriteNumberFont2(0xa0, 0x78, gUnknown_0849B018->unk1b);
}
asm(".global sub_08030C54\n.thumb_set sub_08030C54, LinkDrawTimeoutCounters\n");
