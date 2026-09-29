#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802361C.
 * sub_0802361C @ 0x0802361C
 */

#include "hardware.h"

/* Steps the gUnknown_030033E4 cursor cell one square in the direction the high
 * nibble of gpKeySt->repeated's low byte selects, using W33-D's gUnknown_08499C7C
 * (dx, dy) table, and drags the pixel-space gUnknown_030032C4 four times as far
 * -- the same pairing DesignRoomHandleCursorInput uses. Each axis is bounds-checked against
 * the map header's own width/height and beeps only when the axis actually
 * moved, so a diagonal beeps twice.
 *
 * gUnknown_080909FC, gUnknown_08090A00 and gUnknown_08090A04 in the asm are
 * agbcc -fforce-addr .rodata words holding 0x08499590, 0x030033E4 and
 * 0x08499C7C, not objects; gUnknown_08090A14 in UpdateMapBgScroll is a second
 * private copy of the first of them. The map header is reached as a `u8 *`
 * with `*(u16 *)(p + k)` reads, which is the spelling c_0800119C.c and
 * c_08001158.c already use for gUnknown_08499590.
 *
 * The guard is an early return and not a wrapping `if`: its branch goes
 * straight to the epilogue that both later bounds failures also reach. */
void HandleGameMapCursorInput(void)
{
    int dir;
    int n;

    if ((gMap->unk10 & 0xf) != 0)
        return;

    dir = (gpKeySt->repeated >> 4) & 0xf;

    n = gUnknown_030033E4.unk00 + gUnknown_08499C7C[dir][0];
    if (n >= 0 && n < gMap->width)
    {
        gUnknown_030032C4.unk00 += gUnknown_08499C7C[dir][0] * 4;
        gUnknown_030033E4.unk00 = n;
        if (gUnknown_08499C7C[dir][0] != 0)
            PlayMusicOrSfx2(0x6a);
    }

    n = gUnknown_030033E4.unk02 + gUnknown_08499C7C[dir][1];
    if (n >= 0 && n < gMap->height)
    {
        gUnknown_030032C4.unk02 += gUnknown_08499C7C[dir][1] * 4;
        gUnknown_030033E4.unk02 = n;
        if (gUnknown_08499C7C[dir][1] != 0)
            PlayMusicOrSfx2(0x6a);
    }
}
asm(".global sub_0802361C\n.thumb_set sub_0802361C, HandleGameMapCursorInput\n");
