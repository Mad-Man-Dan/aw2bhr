#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B4D8.
 * sub_0805B4D8 @ 0x0805B4D8, sub_0805B5BC @ 0x0805B5BC
 */

/* Wave 48, W48-B.  Walks the gUnknown_02029ED8 record's 0xFF-terminated index
 * chain (see that symbol's comment for the layout and for why the head byte is
 * a flat index while the +0x60/+0x61 record bytes need the struct cast) and
 * returns the first entry whose cell's low-5-bit terrain code is 8 and which
 * IsTerrainOwnedByUnitsTeam accepts.  0xFE in the record's x byte means "advance the head
 * cursor", not "stop" -- it re-seeds i from head[a1][j] and falls into the
 * shared `i != 0xff` test.
 *
 * NOTE: this match needs its .rodata pool word placed --
 *   "rodata": ["0x0816D9C0"]   (that word holds &gUnknown_030033EC)
 */

struct Unk2029ED8Rec
{
    /* 0x00 */ u8 filler_00[0x60];
    /* 0x60 */ u8 x;
    /* 0x61 */ u8 y;
};

u8 AiFindEnemyHqInInterestList(int a1, int *outX, int *outY)
{
    struct Unk2029ED8Rec *p;
    int i;
    int j;
    int x;
    int y;

    j = 0;

    i = gUnknown_02029ED8[a1 * 0x20 + j + gUnknown_030033EC * 0xc00];

    while (i != 0xff)
    {
        p = (struct Unk2029ED8Rec *)&gUnknown_02029ED8[i * 8 + a1 * 0x3e0
                                                       + gUnknown_030033EC
                                                             * 0xc00];
        x = p->x;

        if (x == 0xfe)
        {
            j++;
            if (j > 0x1f)
                return 0;
            i = gUnknown_02029ED8[j + a1 * 0x20 + gUnknown_030033EC * 0xc00];
            continue;
        }

        y = p->y;

        if ((gMap
                 ->terrain[gMap
                               ->rowOffset[y]
                           + x]
             & 0x1f)
            == 8)
        {
            if (IsTerrainOwnedByUnitsTeam(gUnknown_03003F38,
                             gMap
                                 ->terrain[gMap
                                               ->rowOffset[y]
                                           + x])
                == 0)
            {
                *outX = x;
                *outY = y;
                return 1;
            }
        }

        i++;
    }

    return 0;
}
asm(".global sub_0805B4D8\n.thumb_set sub_0805B4D8, AiFindEnemyHqInInterestList\n");

int AiNextEnemyPropertyInInterestList(int *a1, int *a2, int *outX, int *outY)
{
    struct Unk2029ED8Rec *p;
    int i;
    int x;
    int y;

    i = gUnknown_02029ED8[*a2 + *a1 * 0x20 + gUnknown_030033EC * 0xc00];

    while (i != 0xff)
    {
        p = (struct Unk2029ED8Rec *)&gUnknown_02029ED8[i * 8 + *a1 * 0x3e0
                                                       + gUnknown_030033EC
                                                             * 0xc00];
        x = p->x;

        if (x == 0xfe)
        {
            ++*a2;
            if (*a2 > 0x1f)
                return 0;
            i = gUnknown_02029ED8[*a2 + *a1 * 0x20
                                  + gUnknown_030033EC * 0xc00];
            continue;
        }

        y = p->y;

        if (IsTerrainOwnedByUnitsTeam(gUnknown_03003F38,
                         gMap
                             ->terrain[gMap
                                           ->rowOffset[y]
                                       + x])
            == 0)
        {
            *outX = x;
            *outY = y;
            return 1;
        }

        i++;
    }

    return 0;
}
asm(".global sub_0805B5BC\n.thumb_set sub_0805B5BC, AiNextEnemyPropertyInInterestList\n");
