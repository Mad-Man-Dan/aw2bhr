#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805701C.
 * sub_0805701C @ 0x0805701C
 */

void PutTileEntryB(u16 chr, u16 offset, u16 pal, u16 flip)
{
    gUnknown_08551A04[offset] = chr + ((pal << 12) | (flip << 10));
}
asm(".global sub_0805701C\n.thumb_set sub_0805701C, PutTileEntryB\n");
