#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FC1C.
 * FindTransportForSelectedUnit @ 0x0805FC1C
 *
 * Not a Xenesis-documented name. The old sub_0805FC1C symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "map.h"

/* Looks through the current army's 64 unit slots (from gUnknown_03003F2C) for
 * a transport that can pick up the selected unit (gUnknown_030040D8): an
 * active, unflagged unit on a cell whose gUnknown_03003340 value is not
 * negative, whose type's transport table accepts the selected unit's type
 * (entry 1 + type) and the terrain under it (entry 0x1a + terrain), and which
 * has a free cargo slot. Types 7 and 0x14 use unk07 and type 0x17 uses
 * unk08; 0x17 also requires bits 3-5 of the selected unit's unk07[2] to be
 * 3. The first match's x/y are written to a2. A transport passed over gets
 * 0x40 or 0x80 in the top two bits of unk09.
 *
 * Measured spelling notes (parked from wave 55; wave 78 at 336/328, +8):
 * - One shared `found:` exit for both switch arms, as the ROM has it. That
 *   keeps the loop small enough at loop-optimisation time for LICM to hoist
 *   the unit-type table base (&gUnknown_085D5ABC[0].transportTable, the
 *   ROM's gUnknown_085D5AD0), the park's residual. Two separate
 *   store-and-return blocks were over the threshold.
 * - Each table lookup forms its row pointer in its own statement
 *   (`entry = table + 1;`); inline, the +1 folds into the load.
 * - Found by decomp-permuter from the 97.9% draft: `types = table + 0x1a;
 *   entry = types;` and the `zero` local in the unk08 test, both needed.
 */
void FindTransportForSelectedUnit(int a1, void *a2)
{
    int i;
    int terrain;
    int zero;
    struct Unit *unit;
    struct Unk030040D8 *sel;
    u8 *table;
    u8 *entry;
    u8 *types;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        unit = &gUnits[i];
        if (unit->type == 0)
            continue;
        if (unit->flags & 8)
            continue;
        if ((s8)gUnknown_03003340[unit->y][unit->x] < 0)
            continue;
        table = gUnknown_085D5ABC[unit->type].transportTable;
        sel = gUnknown_030040D8;
        entry = table + 1;
        if (entry[sel->unk00] == 0)
            continue;
        terrain = gMap->terrain[gMap->rowOffset[unit->y] + unit->x] & 0x1f;
        types = table + 0x1a;
        entry = types;
        if (entry[terrain] == 0)
            continue;

        switch (unit->type)
        {
        case 7:
        case 0x14:
            if ((unit->unk09 & 0xc0) == 0 && unit->unk07 == 0)
                goto found;
            unit->unk09 = (unit->unk09 & 0x3f) | 0x40;
            break;
        case 0x17:
            if ((sel->unk07[2] & 0x38) != 0x18)
                continue;
            zero = 0;
            if ((unit->unk09 & 0xc0) != 0x80 && unit->unk08 == zero)
                goto found;
            unit->unk09 = (unit->unk09 & 0x3f) | 0x80;
            break;
        }
    }
    return;

found:
    ((union Unk802C57CBuf *)a2)->pos.unk00 = unit->x;
    ((union Unk802C57CBuf *)a2)->pos.unk02 = unit->y;
}

asm(".global sub_0805FC1C\n.thumb_set sub_0805FC1C, FindTransportForSelectedUnit\n");
