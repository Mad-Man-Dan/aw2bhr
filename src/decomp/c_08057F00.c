#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057F00.
 * CountUnitsWithTypeTag @ 0x08057F00, CountUnitsByDeployLocation @ 0x08057F54, CountUnitsOfType @ 0x08057FA8, AiCountEnemyUnitsOfType @ 0x08057FE8, AiAppendSiloCandidates @ 0x08058058, AiClaimTerritoryCandidate @ 0x08058144
 */

/* c_0804151C.c's list builder again, appending to the gUnknown_03003F20 list
 * from the element index it is HANDED and returning the index it reached: the
 * caller's running count in and out. The same
 * `if ((u8)(gUnknown_030040D8->unk00 - 1) > 1) return 0;` guard as
 * BuildCapturableCellList, and it sits AFTER the cursor is formed, as it does there.
 *
 * The cursor is formed in TWO statements. `(struct Cell *)gUnknown_03003F20 + n`
 * as one expression scales n before loading the pointer
 * (`lsls r1,r7,#2; ldr r0,[r0]; adds r2,r0,r1`); the ROM loads first
 * (`ldr r2,[r0]; lsls r0,r7,#2; adds r2,r2,r0`), which is the separate `+=`.
 *
 * Predicate: the map cell's low 5 terrain bits equal 0x11. The value written is
 * the cell's own signed terrain byte biased by 5 and floored at 0 -- `<= 4`
 * stores 0 on the FALLTHROUGH arm and the `bgt` goes to the `- 5` arm, so the
 * comparison is written that way round.
 *
 * Cell addressing uses gMap's terrain and rowOffset fields directly.
 */

struct Unk58058Cell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};
/* The 4-byte record AiPopFirstNearestCandidate hands back: a (column, row) pair plus the
 * s16 priority it stamps with 0x7FFF on the way out.  It has no struct tag in
 * include/ yet -- gUnknown_03003F20's declared `struct Unk03003338 *` is a
 * different, 0x08-strided type -- so it is spelled locally here. */
struct Unk08057EC0Rec
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
};

int CountUnitsWithTypeTag(int a1)
{
    int i;
    int count;

    count = 0;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        if (gUnits[i].type != 0
         && gUnknown_085D5ABC[gUnits[i].type].unk1b == a1)
            count++;
    }

    return count;
}

asm(".global sub_08057F00\n.thumb_set sub_08057F00, CountUnitsWithTypeTag\n");

int CountUnitsByDeployLocation(int a1)
{
    int i;
    int count;

    count = 0;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        if (gUnits[i].type != 0
         && (gUnknown_085D5ABC[gUnits[i].type].deployLocation & a1) != 0)
            count++;
    }

    return count;
}

asm(".global sub_08057F54\n.thumb_set sub_08057F54, CountUnitsByDeployLocation\n");

int CountUnitsOfType(int a1)
{
    int i;
    int count;

    count = 0;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        if (gUnits[i].type == a1)
            count++;
    }

    return count;
}

asm(".global sub_08057FA8\n.thumb_set sub_08057FA8, CountUnitsOfType\n");

int AiCountEnemyUnitsOfType(int a1)
{
    int i;
    int j;
    int count;

    count = 0;

    for (i = 0; i < 4; i++)
    {
        if ((gPlayers[gUnknown_030033EC].unk2c >> i) & 1)
        {
            for (j = i * 64; j < i * 64 + 0x40; j++)
            {
                if (gUnits[j].type == a1)
                    count++;
            }
        }
    }

    return count;
}
asm(".global sub_08057FE8\n.thumb_set sub_08057FE8, AiCountEnemyUnitsOfType\n");

int AiAppendSiloCandidates(int n)
{
    struct Unk58058Cell *out;
    int off;
    int x;
    int y;

    out = (struct Unk58058Cell *)gUnknown_03003F20;
    out += n;

    if ((u8)(gUnknown_030040D8->unk00 - 1) > 1)
        return 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0)
            {
                off = gMap->rowOffset[y] + x;
                if ((gMap->terrain[off] & 0x1f) == 0x11)
                {
                    n++;
                    out->x = x;
                    out->y = y;
                    if ((s8)gUnknown_03003340[y][x] <= 4)
                        out->v = 0;
                    else
                        out->v = (s8)gUnknown_03003340[y][x] - 5;
                    out++;
                }
            }
        }
    }

    out->v = 0xFFFF;
    return n;
}
asm(".global sub_08058058\n.thumb_set sub_08058058, AiAppendSiloCandidates\n");

/* Repeatedly pulls the next candidate cell off AiPopFirstNearestCandidate and maps it through
 * gMap's rowOffset table into the property plane; that byte selects a
 * gUnknown_084995A0 record. The first
 * record whose unk03[a2] is not above a1 wins: the counter is bumped and the
 * record returned.  NOTE the return type -- the object really is a
 * `struct PropertyListEntry *`; see the comment in include/unknown-functions.h.
 *
 * `arrp` binds gUnknown_084995A0's ADDRESS rather than dereferencing it in
 * place.  That is what puts the `ldr r6, =...` in the loop preheader: written
 * as a bare `gUnknown_084995A0[v]` the address load stays inside the loop, and
 * the pool word moves with it. */
struct Unit *AiClaimTerritoryCandidate(int a1, int a2)
{
    struct Unk08057EC0Rec *e;
    struct PropertyListEntry **arrp;
    struct PropertyListEntry *q;
    int idx;
    int v;

    arrp = &gUnknown_084995A0;

    do
    {
        e = (struct Unk08057EC0Rec *)AiPopFirstNearestCandidate();
        if (e == 0)
            return 0;

        idx = gMap->rowOffset[e->unk01] + e->unk00;
        v = gMap->property[idx];
        q = &(*arrp)[v];
    } while (q->unk03[a2] > a1);

    q->unk03[a2]++;

    return (struct Unit *)q;
}
asm(".global sub_08058144\n.thumb_set sub_08058144, AiClaimTerritoryCandidate\n");
