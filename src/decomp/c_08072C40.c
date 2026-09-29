#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072C40.
 * sub_08072C40 @ 0x08072C40
 */

/* The BG scroll-shadow publisher: `which` selects one of the four BG pairs and
 * the x/y values are stored as halfwords. The four arms share their second
 * store -- agbcc cross-jumps the tail, which is why every arm ends `ldr r0,
 * =<y shadow>` and falls into one common `strh r2, [r0]`.
 *
 * The shadows are the wave-12 BGxHOFS/BGxVOFS pairs; several are volatile,
 * which is why each arm loads its own pool word instead of walking one
 * address. A selector outside 0..3 stores nothing. */
void SetBgScrollShadow(u16 which, u16 x, u16 y)
{
    switch (which)
    {
    case 0:
        gUnknown_03001FF8 = x;
        gUnknown_03001418 = y;
        break;

    case 1:
        gUnknown_03002B34 = x;
        gUnknown_03002F18 = y;
        break;

    case 2:
        gUnknown_030030A0 = x;
        gUnknown_03001400 = y;
        break;

    case 3:
        gUnknown_0300200C = x;
        gUnknown_03002000 = y;
        break;
    }
}
asm(".global sub_08072C40\n.thumb_set sub_08072C40, SetBgScrollShadow\n");
