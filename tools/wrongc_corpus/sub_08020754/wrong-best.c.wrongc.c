#include "global.h"
#include "map.h"

#define MAP ((struct Map *)gUnknown_08499590)

void sub_08020754(u8 *dst)
{
    int x, y;

    for (y = 0; y < MAP->height; y++)
    {
        for (x = 0; x < MAP->width; x++)
        {
            if (((MAP->terrain[MAP->rowOffset[y] + x] & 0x1f) == 4 || (MAP->terrain[MAP->rowOffset[y] + x] & 0x1f) == 0x13)
             && (MAP->unitUnk[MAP->rowOffset[y] + x] == 0 || (u8)(gUnknown_08499594[MAP->unitUnk[MAP->rowOffset[y] + x]].type - 0x10) > 4))
                goto zero;
            dst[MAP->rowOffset[y] + x] = 1;
            continue;
        zero:
            dst[MAP->rowOffset[y] + x] = 0;
        }
    }
}
