#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D5E8.
 * sub_0802D5E8 @ 0x0802D5E8
 */

void OpenDeploymentScreen(s16 a1, s16 a2)
{
    int off;
    int v;

    v = 0;
    off = gMap->rowOffset[a2];
    off = a1 + off;

    switch (gMap->terrain[off] & 0x1f)
    {
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_BASE:
        v = 7;
        break;

    case TERRAIN_AIRPORT:
        v = 0x10;
        break;

    case TERRAIN_PORT:
        v = 0x20;
        break;
    }

    BuildDeploymentList(v);
    sub_080152C0((s32)gUnknown_0849AFE8, 0);
    IncrementMapLock();
}
asm(".global sub_0802D5E8\n.thumb_set sub_0802D5E8, OpenDeploymentScreen\n");
