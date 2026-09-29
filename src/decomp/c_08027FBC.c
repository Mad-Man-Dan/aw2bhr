#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027FBC.
 * sub_08027FBC @ 0x08027FBC
 */

/* A 0x200-byte VRAM push between two tile numbers.
 *
 * `lsls #0x10; lsrs #0xc` is PROMOTE_MODE's zero-extension of a u16 parameter
 * FUSED by combine with `* 0x10` -- net left shift of four with the top half
 * cleared, three instructions collapsed into two. An `int` parameter cannot
 * produce it, so a2 and a3 are u16. The `& 0x3ff` then `<< 5` is the usual
 * tile-index wrap times 0x20 bytes per tile, and `movs r1,#0xe5; lsls r1,#1`
 * is just the constant 0x1ca. */
void CopyDayGlyphTiles(void *a1, u16 a2, u16 a3)
{
    RegisterDataMove((u8 *)a1 + (((a2 * 0x10) & 0x3ff) << 5),
                 (void *)((((a3 * 0x10 + 0x1ca) & 0x3ff) << 5) + 0x06010000),
                 0x200);
}
asm(".global sub_08027FBC\n.thumb_set sub_08027FBC, CopyDayGlyphTiles\n");
