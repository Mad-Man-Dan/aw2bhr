#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080236E8.
 * sub_080236E8 @ 0x080236E8
 */

#include "map.h"
#include "hardware.h"

void HandleMoveMapCursorInMoveRange(void)
{
    s16 (*tbl)[2];
    u16 keys;
    int dir;
    int kk;
    int n;
    struct Unk802C57C next;

    if ((gMap->unk10 & 0xf) != 0)
        return;

    kk = gpKeySt->repeated;
    keys = kk;
    dir = (keys >> 4) & 0xf;

    next.unk00 = gUnknown_030033E4.unk00 + gUnknown_08499C7C[dir][0];
    next.unk02 = gUnknown_030033E4.unk02 + gUnknown_08499C7C[dir][1];

    if (gMap->move[gMap->rowOffset[gUnknown_030033E4.unk02] + gUnknown_030033E4.unk00] > 0
     && (u8)gMap->move[gMap->rowOffset[next.unk02] + next.unk00] > 0x78
     && keys != gpKeySt->pressed)
        return;

    n = gUnknown_030033E4.unk00 + gUnknown_08499C7C[dir][0];
    if (n >= 0 && n < gMap->width)
    {
        gUnknown_030032C4.unk00 += gUnknown_08499C7C[dir][0] * 4;
        gUnknown_030033E4.unk00 = n;
        if ((tbl = gUnknown_08499C7C)[dir][0] != 0)
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
asm(".global sub_080236E8\n.thumb_set sub_080236E8, HandleMoveMapCursorInMoveRange\n");
