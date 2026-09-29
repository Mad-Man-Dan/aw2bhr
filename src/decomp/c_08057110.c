#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057110.
 * sub_08057110 @ 0x08057110
 */

void PutTileEntryA(u16 chr, u16 offset, u16 pal, u16 flip)
{
    gUnknown_08551A00[offset] = chr + ((pal << 12) | (flip << 10));
}
asm(".global sub_08057110\n.thumb_set sub_08057110, PutTileEntryA\n");
