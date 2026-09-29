#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A268.
 * sub_0805A268 @ 0x0805A268, sub_0805A388 @ 0x0805A388
 */

/* AiListEmbarkBoundUnits's sibling (src/decomp/c_0805A514.c) and a producer for the same
 * cell list AiFillLanderCostToShoalPlane consumes: sweeps the 0x40-unit window at
 * gUnknown_03003F2C and emits {x, y, threat} for every unit that passes, then
 * terminates the array with 0xFFFF exactly as AiListEmbarkBoundUnits does.
 *
 * Two disjoint acceptance arms sharing one emit body. The first takes units
 * HasSupplyAbility accepts whose class byte is live in gUnknown_084995A8; the
 * second is the fallback and only opens when the active player's class byte is
 * 0x13 or 0x14 -- spelled as the ROM's `(u8)(c - 0x13) <= 1`, one subtract and
 * a byte truncation rather than two compares. THE EMIT BODY IS WRITTEN OUT
 * TWICE ON PURPOSE: agbcc cross-jumps the two copies and the shared tail it
 * produces (from the second `u->y` reload onward, reached by a `b`) is
 * exactly the ROM's. A single shared body cannot produce that layout.
 *
 * NEITHER gUnknown_0816D984 NOR gUnknown_0816D988 IS A GLOBAL. The ROM words
 * hold 0x030040D8 and 0x03003F2C -- the ADDRESSES of gUnknown_030040D8 and
 * gUnknown_03003F2C, checked in baserom.gba. Both are -fforce-addr copies (each
 * object is read on both sides of a merge), the pattern c_0805A95C.c
 * documents. Named honestly here; promotion must carry
 * "rodata": ["0x0816D984", "0x0816D988"].
 *
 * `flag` holds 0 / -1 rather than 0 / 1: the ROM builds it with
 * `movs r1,#1; rsbs r1,r1,#0`, so the original wrote -1.
 *
 * MATCHED, 1 attempt after 1 probe. */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void AiListSuppliersInReach(struct Unk5A514Cell *out)
{
    struct Unit *u;
    int i;
    int flag;

    flag = 0;
    if ((u8)(gUnknown_030040D8->unk00 - 0x13) <= 1)
        flag = -1;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        u = &gUnits[i];
        if (u->type == 0)
            continue;

        if (HasSupplyAbility((u8 *)u) && gUnknown_084995A8[gUnknown_030040D8->unk00] != 0)
        {
            if ((s8)gUnknown_03003340[u->y][u->x] < 0)
                continue;
            if (u->flags & 8)
                continue;
            out->x = u->x;
            out->y = u->y;
            out->v = (s8)gUnknown_03003340[u->y][u->x];
            out++;
        }
        else
        {
            if (flag == 0)
                continue;
            if (u->type != 0x16)
                continue;
            if (u->unk08 != 0)
                continue;
            if ((s8)gUnknown_03003340[u->y][u->x] < 0)
                continue;
            out->x = u->x;
            out->y = u->y;
            out->v = (s8)gUnknown_03003340[u->y][u->x];
            out++;
        }
    }

    out->v = 0xFFFF;
}
asm(".global sub_0805A268\n.thumb_set sub_0805A268, AiListSuppliersInReach\n");

void AiListSuppliersAndSupplyProperties(struct Unk5A514Cell *out)
{
    struct Unit *u;
    u8 *tbl;
    int x;
    int y;
    int off;
    int flag;
    int key;

    flag = 0;
    key = gUnknown_08576841[gUnknown_030040D8->unk00];
    if ((u8)(gUnknown_030040D8->unk00 - 0x13) <= 1)
        flag = -1;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;

            off = gMap->rowOffset[y] + x;

            if (gMap->unit[off] != 0)
            {
                if ((gMap->unit[off] & 0xc0) != gUnknown_03003F2C)
                    continue;
                u = &gUnits[gMap->unit[off]];
                if (flag == 0 && HasSupplyAbility((u8 *)u)
                 && gUnknown_084995A8[gUnknown_030040D8->unk00] != 0)
                {
                    out->x = x;
                    out->y = y;
                    out->v = (s8)gUnknown_03003340[y][x];
                    out++;
                }
                else
                {
                    if (flag == 0)
                        continue;
                    if (u->type != 0x16)
                        continue;
                    if (u->unk07 != 0)
                        continue;
                    out->x = x;
                    out->y = y;
                    out->v = (s8)gUnknown_03003340[y][x];
                    out++;
                }
            }
            else
            {
                tbl = gUnknown_0857685A;
                if (tbl[gMap->terrain[off] & 0x1f] != key)
                    continue;
                if ((gMap->terrain[off] & 0xe0) != gUnknown_03004084)
                    continue;
                out->x = x;
                out->y = y;
                out->v = (s8)gUnknown_03003340[y][x];
                out++;
            }
        }
    }

    out->v = 0xFFFF;
}
asm(".global sub_0805A388\n.thumb_set sub_0805A388, AiListSuppliersAndSupplyProperties\n");
