#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062474.
 * sub_08062474 @ 0x08062474
 */

void sub_08062474(void)
{
    int army;
    int saved;
    u8 mask;

    army = gUnknown_03004480;
    saved = army;

    if (gUnknown_030045CC.unk00_0)
        return;
    gUnknown_030045CC.unk00_0 = 1;

    SetWorkingMapPlane(gMap->move);
    sub_080581A4(gMap->dangerMask, 0);

    mask = gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d;

    if (gPlayers[army].unk2c & 1)
        sub_08062560(1, mask);
    if (gPlayers[army].unk2c & 2)
        sub_08062560(2, mask);
    if (gPlayers[army].unk2c & 4)
        sub_08062560(3, mask);
    if (gPlayers[army].unk2c & 8)
        sub_08062560(4, mask);

    gUnknown_03004480 = saved;
}
