#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022580.
 * sub_08022580 @ 0x08022580
 */

/* gUnknown_080909A4 in the asm is NOT a global: the ROM word at 0x080909A4
 * holds 0x08499590, i.e. it is agbcc's own -fforce-addr address constant for
 * gUnknown_08499590.  Same for gUnknown_080909B0 (RedrawUnitIconLayer),
 * gUnknown_08090978 (RenderMap) and gUnknown_0809099C; gUnknown_080909A0
 * holds 0x08499594.  Writing the honest name reproduces the whole chain:
 * `ldr rX,=.LC0; ldr rY,[rX]; ldr rZ,[rY]` is &word -> &global -> map pointer.
 *
 * +4 / +6 are the camera's pixel scroll, unwrapped to tile units by
 * `(s16)v >> 4`; the ldrh + lsls #0x10 + asrs #0x14 is that cast on a u16
 * member, not an s16 load. */
void RedrawUnitLayer(void)
{
    u16 x;
    u16 y;

    for (y = 0; y <= 0xf; y++)
    {
        for (x = 0; x <= 0xf; x++)
        {
            DrawUnitAt((u16)(x + (gMap->scrollX >> 4)),
                         (u16)(y + (gMap->scrollY >> 4)));
        }
    }

    BG_EnableSyncBG1();
}
asm(".global sub_08022580\n.thumb_set sub_08022580, RedrawUnitLayer\n");
