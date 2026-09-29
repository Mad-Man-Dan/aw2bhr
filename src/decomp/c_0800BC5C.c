#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800BC5C.
 * sub_0800BC5C @ 0x0800BC5C, sub_0800BC98 @ 0x0800BC98
 */

/* Reports whether the terrain at (x, y) is one of the two bridge codes. Same
 * struct-through-a-cast reading of gUnknown_08499590 as sub_0800B1FC -- see
 * that function for why the flat pointer spelling does not match. */
int CanPlaceReefAt(int x, int y)
{
    struct Map *s = gMap;
    u8 t = s->terrain[s->rowOffset[y] + x];

    if (t == 0x13 || t == 7)
        return 1;
    else
        return 0;
}
asm(".global sub_0800BC5C\n.thumb_set sub_0800BC5C, CanPlaceReefAt\n");

int IsSeaAt(int x, int y)
{
    struct Map *s = gMap;

    if (s->terrain[s->rowOffset[y] + x] != 7)
        return 0;
    else
        return 1;
}
asm(".global sub_0800BC98\n.thumb_set sub_0800BC98, IsSeaAt\n");
