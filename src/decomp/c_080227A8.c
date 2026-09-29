#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080227A8.
 * sub_080227A8 @ 0x080227A8
 */

/* The sub_08022618 twin of RedrawUnitLayer -- identical body, three names
 * substituted.  gUnknown_080909B0 in the asm is not a global: the ROM word at
 * 0x080909B0 holds 0x08499590, agbcc's own -fforce-addr address constant for
 * gUnknown_08499590, so the honest name reproduces the two-level chain.
 *
 * +4 / +6 are the camera's pixel scroll, unwrapped to tile units by
 * `(s16)v >> 4`; the ldrh + lsls #0x10 + asrs #0x14 is that cast on a u16
 * member, not an s16 load. */
void RedrawUnitIconLayer(void)
{
    u16 x;
    u16 y;

    for (y = 0; y <= 0xf; y++)
    {
        for (x = 0; x <= 0xf; x++)
        {
            sub_08022618((u16)(x + (gMap->scrollX >> 4)),
                         (u16)(y + (gMap->scrollY >> 4)));
        }
    }

    BG_EnableSyncBG2();
}
asm(".global sub_080227A8\n.thumb_set sub_080227A8, RedrawUnitIconLayer\n");
