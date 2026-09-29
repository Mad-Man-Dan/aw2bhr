#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023518.
 * sub_08023518 @ 0x08023518
 */

#include "hardware.h"

/* MATCHED (wave 81, W81-D). The wave-36 blocker -- "you get (CSE +
 * force-addr) or (reload + inline pool), never (reload + force-addr)" --
 * was a HEADER question: gUnknown_08499C7C is not const in the original
 * source. With include/unknown-globals.h de-consted, the table reads stop
 * being RTX_UNCHANGING, so every arm reloads after the intervening strh
 * exactly as the ROM does, while the -fforce-addr .rodata words survive.
 * Verified under the same header change: promoted HandleGameMapCursorInput, UpdateMapBgScroll
 * and DesignRoomHandleCursorInput still byte-match, and HandleMoveMapCursorInMoveRange improves to 76.3%. */

void MoveMapCursorFromHeldKeys(void)
{
    int dir;
    int n;

    if ((gMap->unk10 & 0xf) != 0)
        return;

    dir = (gpKeySt->held >> 4) & 0xf;

    if (gUnknown_08499C7C[dir][0] < 0)
        gUnknown_030033E4.unk00 = (gMap->scrollX >> 4) + 2;
    if (gUnknown_08499C7C[dir][0] > 0)
        gUnknown_030033E4.unk00 = (gMap->scrollX >> 4) + 0xc;
    if (gUnknown_08499C7C[dir][1] < 0)
        gUnknown_030033E4.unk02 = (gMap->scrollY >> 4) + 2;
    if (gUnknown_08499C7C[dir][1] > 0)
        gUnknown_030033E4.unk02 = (gMap->scrollY >> 4) + 7;

    n = gUnknown_030033E4.unk00 + gUnknown_08499C7C[dir][0];
    if (n >= 0 && n < gMap->width)
    {
        gUnknown_030032C4.unk00 += gUnknown_08499C7C[dir][0] * 4;
        gUnknown_030033E4.unk00 = n;
    }

    n = gUnknown_030033E4.unk02 + gUnknown_08499C7C[dir][1];
    if (n >= 0 && n < gMap->height)
    {
        gUnknown_030032C4.unk02 += gUnknown_08499C7C[dir][1] * 4;
        gUnknown_030033E4.unk02 = n;
    }
}
asm(".global sub_08023518\n.thumb_set sub_08023518, MoveMapCursorFromHeldKeys\n");
