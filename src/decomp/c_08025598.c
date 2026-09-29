#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025598.
 * sub_08025598 @ 0x08025598
 */

u8 IsViewerUnitAtCell(s16 a1, s16 a2)
{
    int off;
    int id;

    off = gMap->rowOffset[a2] + a1;
    id = gMap->unitUnk[off];

    if ((u16)id == 0)
        return 0;

    if (gPlayers[((u16)id >> 6) + 1].turnState & 2)
        return 1;

    return 0;
}
asm(".global sub_08025598\n.thumb_set sub_08025598, IsViewerUnitAtCell\n");
