#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010ADC.
 * sub_08010ADC @ 0x08010ADC
 */

/* Repaints the cell (x, y) and its four orthogonal neighbours, skipping any
 * that falls outside the map. The offsets come from a pair of five-entry s16
 * direction tables -- gUnknown_0848897C = { 0, -1, 1, 0, 0 } (dx) and
 * gUnknown_08488986 = { 0, 0, 0, -1, 1 } (dy), both dereferenced in
 * baserom.gba -- so index 0 is the cell itself and 1..4 are left, right, up
 * and down.
 *
 * They are named as two separate arrays rather than one 2x5 table: the ROM
 * materialises dy through its own inline pool word instead of indexing off
 * dx's base register, which a `tbl[1][i]` spelling cannot produce.
 *
 * The word at 0x0808D8A0 is NOT a global -- it is agbcc's own -fforce-addr
 * .rodata address constant holding 0x0848897C, one slot before the
 * 0x0808D8A4/0x0808D8A8 pair c_08010B34.c documents for &gUnknown_08499590.
 * The honest spelling below reproduces the two-level `ldr r7,=<word>` /
 * `ldr r0,[r7]` load and promotion must carry the rodata entry. */
void RepaintPipesAround(int x, int y)
{
    int i;
    int nx;
    int ny;

    for (i = 0; i < 5; i++)
    {
        nx = x + gUnknown_0848897C[i];
        if (nx >= 0 && nx < gMap->width)
        {
            ny = y + gUnknown_08488986[i];
            if (ny >= 0 && ny < gMap->height)
                sub_08010664(nx, ny);
        }
    }
}
asm(".global sub_08010ADC\n.thumb_set sub_08010ADC, RepaintPipesAround\n");
