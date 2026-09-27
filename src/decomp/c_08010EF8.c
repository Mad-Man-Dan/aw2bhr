#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010EF8.
 * DrawNumberRightAligned @ 0x08010EF8
 *
 * Not a Xenesis-documented name. The old sub_08010EF8 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

/* Writes `value` in decimal into the tilemap `dest`, right-aligned so that
 * its last digit lands at column x of the second row (cell x + 0x20). The
 * digits go right to left, one tile per digit, and each tile is the digit
 * glyph base gUnknown_0300308C[0] plus the digit. Zero draws a single "0".
 * The `unused` parameter is never read.
 *
 * Spelling note: the digit comes first in the sum. Written
 * `gUnknown_0300308C[0] + value % 10`, agbcc loads &gUnknown_0300308C from
 * the text pool once, ahead of the loop, and reads only the `ldrh` inside it
 * (one instruction short in the loop, same size overall). With the operands
 * swapped, the address goes through the -fforce-addr .rodata word
 * (0x0808DF8C) and is re-read every iteration (`ldr r1,[r6]; ldrh r1,[r1]`),
 * as in the ROM. That is the W49-M operand-order lever. The ten declaration
 * spellings tried in waves 42-58 all looked for a pointer global at
 * 0x0808DF8C. That word is just the force-addr word for gUnknown_0300308C.
 */
void DrawNumberRightAligned(u16 x, int unused, u16 value, u16 *dest)
{
    dest += x + 0x20;
    do
    {
        *dest = value % 10 + gUnknown_0300308C[0];
        dest--;
        value = value / 10;
    } while (value != 0);
}

asm(".global sub_08010EF8\n.thumb_set sub_08010EF8, DrawNumberRightAligned\n");
