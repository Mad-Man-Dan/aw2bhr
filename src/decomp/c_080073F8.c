#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080073F8.
 * sub_080073F8 @ 0x080073F8, sub_080077EC @ 0x080077EC
 */

/* Rebuilds the design-room item ring for one side (0 = terrain list,
 * 1 = unit list): clears every entry's bit 0, then lays out `limit` entries
 * starting at gActiveMap->ringIndex, each taking its item from the
 * side's list in gUnknown_0200B224 (both indices wrap). For side 0, a
 * `selected` code with an army bit pattern ORs that army into each item
 * and reloads the ring through sub_080077EC / sub_08007B74.
 *
 * Matching notes:
 *  - The item copy is written in BOTH editMode arms. The two identical
 *    `gUnknown_0200B224[list]` givs combine, which makes loop.c's
 *    strength reduction profitable against list's three increments (+1,
 *    -17, -20): that is the ROM's walking r3 pointer, initialised after
 *    the loop guard. jump.c then cross-jumps the two stores into one
 *    block. That merge point has two predecessors, so CSE loses the stored
 *    value and the army OR re-reads entry->itemId, as the ROM does.
 *  - army/armyIndex are declared before count/limit: that sets the order
 *    of the spilled stack slots (armyIndex sp+4, count sp+8, limit sp+12). */
void DesignRoomBuildRing(int side, int selected)
{
    int army = -1;
    int armyIndex = 0;
    int i;
    int ring;
    int list;
    int count;
    int limit;

    if (side == 0)
    {
        switch (selected)
        {
        case 6: case 8: case 10: case 11: case 14:
            army = 0; armyIndex = 0; break;
        case 38: case 40: case 42: case 43: case 46:
            army = 0x20; armyIndex = 1; break;
        case 70: case 72: case 74: case 75: case 78:
            army = 0x40; armyIndex = 2; break;
        case 102: case 104: case 106: case 107: case 110:
            army = 0x60; armyIndex = 3; break;
        case 134: case 136: case 138: case 139: case 142:
            army = 0x80; armyIndex = 4; break;
        default: army = -1; break;
        }
    }

    for (i = 0; i < 10; i++)
        gDesignRing[i].flags &= ~1;

    ring = gActiveMap->ringIndex;
    list = side == 0 ? gActiveMap->terrainListIndex : gActiveMap->unitListIndex;
    count = side == 0 ? 10 : 8;
    limit = side == 0 ? 9 : 7;

    for (i = 0; i < limit; i++)
    {
        struct DesignRingEntry *entry = &gDesignRing[ring];
        int flags;
        flags = entry->flags & ~1;

        entry->spriteSlot = i;
        entry->flags = flags | 8;
        entry->y = 0x8200;
        if (gActiveMap->editMode == 0) {
            entry->x = gUnknown_084886F8[side][4] << 8;
            entry->targetX = gUnknown_084886F8[side][i];
            entry->xVelocity = 0;
            entry->itemId = gUnknown_0200B224[list].unk00;
        } else {
            entry->x = gUnknown_084886F8[side][3] << 8;
            entry->targetX = gUnknown_084886F8[side][i];
            entry->xVelocity = 0;
            entry->itemId = gUnknown_0200B224[list].unk00;
        }
        if (side == 0 && army >= 0)
            entry->itemId = (entry->itemId & 0x1F) | army;

        list++;
        if (side == 0) {
            if (list > 16) list -= 17;
        } else {
            if (list > 19) list -= 20;
        }
        ring++;
        if (ring >= count)
            ring -= count;
    }

    if (side == 0 && army >= 0)
    {
        sub_080077EC(selected, armyIndex);
        sub_08007B74();
    }
}
asm(".global sub_080073F8\n.thumb_set sub_080073F8, DesignRoomBuildRing\n");

/* Loads army `army`'s five design-ring items (two halfwords each, from
 * gUnknown_084887AC) into gUnknown_0200B224[9..13], then copies their
 * first halfwords into the ring slots starting at ringIndex + an offset
 * picked by `item`'s low five bits (8/6/14/10/11 -> 4..0; any other code
 * keeps the first loop's final i).
 *
 * Matching notes, all load-bearing:
 *  - The first loop is a goto loop, so loop.c never sees it; a real loop
 *    reduces (i - 9) * 10 and reverses. `dst = base + 9` through a separate
 *    `base` local gives the ROM's `ldr r0; adds r2,r0,#0; adds r2,#36`.
 *    The `do { } while (0)` around the two copies, and routing army * 2
 *    through `j`, fix the loop's register assignment.
 *  - The second loop counts j up and indexes through its own `k`: loop.c
 *    reverses j into the ROM's 4..0 counter and reduces
 *    gUnknown_0200B224[k] to the walking r4 pointer, in the ROM's
 *    preheader order. `i++` before `k++` orders the increments.
 *  - `n` is the ROM's dead `terrainListIndex + i` add; the one-trip while
 *    keeps it alive. */
void sub_080077EC(int item, int army)
{
    int i;
    int slot;
    int j;
    const u16 *table;
    int n;
    int k;
    int a2;
    struct Unk0200B224 *base;
    struct Unk0200B224 *dst;

    gActiveMap->propertyArmy = army;
    i = 9;
    base = gUnknown_0200B224;
    table = gUnknown_084887AC;
    j = army * 2;
    dst = base + 9;
    a2 = j;
copy_next:
    {
        int src = (i - 9) * 10;
        src += a2;
        do
        {
            dst->unk00 = table[src];
            src++;
            dst->unk02 = table[src];
        } while (0);
    }

    dst++;
    i++;
    if (i < 14) goto copy_next;

    switch (item & 0x1F)
    {
    case 8: i = 4; break;
    case 6: i = 3; break;
    case 14: i = 2; break;
    case 10: i = 1; break;
    case 11: i = 0; break;
    }

    k = 9;
    for (j = 0; j < 5; j++)
    {
        slot = gActiveMap->ringIndex + i;
        if (slot > 9)
            slot -= 10;
        n = gActiveMap->terrainListIndex + i;
        while (n == 1)
            n = 0;
        gDesignRing[slot].itemId = gUnknown_0200B224[k].unk00;
        i++;
        k++;
    }
}
