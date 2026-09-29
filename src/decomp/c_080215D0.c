#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080215D0.
 * sub_080215D0 @ 0x080215D0
 */

/* Copies 0x400 bytes of ROM over the buffer gUnknown_0849959C points at.
 * Both bases are bound to locals: the pointer global's deref is hoisted out
 * of the loop despite the `strb`, and binding the ROM array as well is what
 * puts its pool word first. */
void LoadTileTerrainTable(void)
{
    u8 *src;
    u8 *dst;
    int i;

    src = gUnknown_080C1BC4;
    dst = gUnknown_0849959C;

    for (i = 0; i <= 0x3ff; i++)
        dst[i] = src[i];
}
asm(".global sub_080215D0\n.thumb_set sub_080215D0, LoadTileTerrainTable\n");
