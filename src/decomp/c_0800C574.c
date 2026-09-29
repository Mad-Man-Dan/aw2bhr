#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800C574.
 * sub_0800C574 @ 0x0800C574, sub_0800C608 @ 0x0800C608, sub_0800C6A8 @ 0x0800C6A8, sub_0800C6E8 @ 0x0800C6E8
 */

void AddPropertyRecord(int x, int y, int t)
{
    int n;
    int off;
    int u;
    int z;

    if ((s8)gActiveMap->propertyCount > 0x5B)
        return;

    n = (s8)gActiveMap->propertyCount;

    off = gMap->rowOffset[y] + x;
    gMap->property[off] = (z = 0, n);

    gUnknown_084995A0[n].unk00 = t;
    gUnknown_084995A0[n].unk01 = x;
    gUnknown_084995A0[n].unk02 = y;
    gUnknown_084995A0[n].unk03[0] = z;
    gUnknown_084995A0[n + 1].unk00 = 0xFF;

    gProperty[n].flags = t;
    gProperty[n].x = x;
    gProperty[n].y = y;
    n++;
    gProperty[n].flags = -1;
    gActiveMap->propertyCount = n;
}
asm(".global sub_0800C574\n.thumb_set sub_0800C574, AddPropertyRecord\n");

/* The REMOVE half of the AddPropertyRecord pair: drops the record for cell (x, y)
 * from the two parallel arrays and decrements the count AddPropertyRecord
 * increments.
 *
 * The cell byte comes off gMap->terrain through gMap->rowOffset, the
 * c_0800C840 idiom -- but UNMASKED here, where GetPropertyKindAt hands
 * GetPropertyKindForTerrain `& 0x1f`.  GetPropertyKindForTerrain masks internally, so the two spellings
 * agree; ClearArmyHq then switches the same raw byte over 0x28/0x48/0x68/0x88,
 * values outside the low five bits, so the mask genuinely is absent here.
 *
 * The scan has TWO exits and the bottom of the loop gives their order: the
 * `cmp r4,#0x5b; bgt` bound runs BEFORE the 0xFF sentinel re-check, so this is
 * a bounded `for` whose body `break`s and not a `while (flags != 0xFF)`.  The
 * index survives as a real variable rather than becoming a pointer giv only
 * because gUnknown_084995A0's 8-byte stride needs it (`lsls r0, r4, #3`); the
 * 4-byte array still gets the giv.
 *
 * `(s8)gActiveMap->propertyCount` on the u8 member emits `movs r0,#0x12;
 * ldrsb r0,[r1,r0]` -- see the note in src/decomp/c_08010B34.c, which this
 * function corrects: the cast folds to ldrsb whenever the address is
 * base+constant, and only leaves `ldrb; lsl; asr` behind when it is not.
 * The decrement one line above reads the SAME member with a plain `ldrb`
 * because a truncating store back needs no sign, and the reload between them
 * is that store aliasing the pointer global. */
void RemovePropertyAt(int x, int y)
{
    int off;
    int cell;
    int i;

    off = gMap->rowOffset[y] + x;
    cell = gMap->terrain[off];

    if (GetPropertyKindForTerrain(cell) == 2)
        ClearArmyHq(cell);

    for (i = 0; i <= 0x5B; i++)
    {
        if (gProperty[i].flags == 0xFF)
            break;
        if (gProperty[i].flags != 0 && gProperty[i].x == x
         && gProperty[i].y == y)
        {
            gUnknown_084995A0[i].unk00 = 0;
            gProperty[i].flags = 0;
            gActiveMap->propertyCount--;
            if ((s8)gActiveMap->propertyCount < 0)
                gActiveMap->propertyCount = 0;
        }
    }
}
asm(".global sub_0800C608\n.thumb_set sub_0800C608, RemovePropertyAt\n");

/* Totals GetArmyHq over all four 0x28/0x68/0x48/0x88 ids.  The two out
 * parameters are one shared pair of stack ints reused by every call -- the ROM
 * computes `add r5, sp, #4` once in the prologue and passes the same `sp` and
 * `r5` four times -- so the values are thrown away and only the count is kept.
 *
 * An accumulator, not a four-term sum: the ROM folds each result into r4 at
 * its own call (`adds r4, r0, #0` then three `adds r4, r4, r0`).  Written as
 * one `a + b + c + d` expression agbcc keeps all four results live to the end,
 * spending r6 and r8 and an extra push/pop pair.
 *
 * The id order is 0x28, 0x68, 0x48, 0x88 -- not the sorted order the switch in
 * the callee uses. */
int sub_0800C6A8(void)
{
    int x;
    int y;
    int n;

    n = GetArmyHq(0x28, &x, &y);
    n += GetArmyHq(0x68, &x, &y);
    n += GetArmyHq(0x48, &x, &y);
    n += GetArmyHq(0x88, &x, &y);

    return n;
}

int GetArmyHq(int a, int *b, int *c)
{
    int i;
    int ok;

    ok = 0;
    switch (a)
    {
    case 0x28:
        i = 0;
        break;
    case 0x48:
        i = 1;
        break;
    case 0x68:
        i = 2;
        break;
    case 0x88:
        i = 3;
        break;
    default:
        return ok;
    }

    if ((s8)gActiveMap->hqX[i] >= 0 && (s8)gActiveMap->hqY[i] >= 0)
    {
        *b = (s8)gActiveMap->hqX[i];
        *c = (s8)gActiveMap->hqY[i];
        ok = 1;
    }

    return ok;
}
asm(".global sub_0800C6E8\n.thumb_set sub_0800C6E8, GetArmyHq\n");
