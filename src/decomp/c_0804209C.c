#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804209C.
 * sub_0804209C @ 0x0804209C, sub_08042154 @ 0x08042154, sub_080421D0 @ 0x080421D0, sub_0804223C @ 0x0804223C, sub_080422A8 @ 0x080422A8, sub_0804236C @ 0x0804236C
 */

/* Is there an enemy unit standing on (x, y) whose displayed stats are stale?
 * The bounds pair, the +0x417A row table and the unit-id plane at +0x51A are
 * the c_08001158.c map-cell idiom; gUnknown_03003F2C holds the army bits the
 * cell's top two must equal.
 *
 * gUnknown_08499590 is read through the GLOBAL in the bounds chain rather than
 * through a local bound before it. CSE still emits one `ldr` and reuses it for
 * everything after, but binding a local first creates the pseudo ahead of the
 * `x < 0` test -- which both hoists the load above that branch and pushes the
 * zero-extended y out of r1 into r4.
 *
 * The two tail compares are the stat check: unk0b caps the 4-bit field at bit 7
 * of the unit's unk04 container and unk10 caps the 7-bit unk06, exactly the
 * pairing gUnknown_085D5ABC's own comments record for sub_08042998. */
bool8 IsResupplyableAllyAt(s16 x, s16 y)
{
    struct Map *p;
    u8 *rows;
    u8 *units;
    struct Unit *unit;
    struct UnitType *e;
    int y2;
    int off;
    int id;

    if (x < 0 || x >= gMap->width
     || y < 0 || y >= gMap->height)
        return FALSE;

    p = gMap;
    y2 = y * 2;
    rows = (u8 *)p->rowOffset;
    off = *(u16 *)(rows + y2) + x;
    units = p->unitUnk;
    id = units[off];

    if (id == 0)
        return FALSE;

    if ((id & 0xc0) != gUnknown_03003F2C)
        return FALSE;

    unit = &gUnits[id];

    if (unit == (struct Unit *)gUnknown_030040D8)
        return FALSE;

    if (gUnknown_084995A8[unit->type] == 0)
        return FALSE;

    e = &gUnknown_085D5ABC[unit->type];

    if (e->maxAmmo != unit->ammo)
        return TRUE;

    if (e->maxFuel != unit->fuel)
        return TRUE;

    return FALSE;
}
asm(".global sub_0804209C\n.thumb_set sub_0804209C, IsResupplyableAllyAt\n");

/* The OR of CanDropFirstCargoAt and CanDropSecondCargoAt: passable if EITHER flag byte
 * clears GetDropDirectionMask. The `||` with an explicit `else` is what puts the
 * `movs r0,#1` in the fallthrough slot after the second `beq`; the chained
 * `if (a) return TRUE; if (b) return TRUE;` spelling emits the zero arm
 * there instead, as CanDropFirstCargoAt does. */
bool8 CanDropAnyCargoAt(struct Unk030040D8 *a1, s16 a2, s16 a3)
{
    u8 *t;
    struct Map *p;
    u8 *rows;
    u8 *cells;
    u8 *costs;
    int y2;
    int off;
    int idx;

    t = gUnknown_085D5ABC[a1->unk00].transportTable;
    p = gMap;
    y2 = a3 * 2;
    rows = (u8 *)p->rowOffset;
    off = *(u16 *)(rows + y2) + a2;
    cells = p->terrain;
    idx = cells[off] & 0x1f;
    costs = t + 0x1a;

    if (costs[idx] == 0)
        return FALSE;

    if (GetDropDirectionMask(a2, a3, a1->unk07[0]) != 0
     || GetDropDirectionMask(a2, a3, a1->unk07[1]) != 0)
        return TRUE;
    else
        return FALSE;
}
asm(".global sub_08042154\n.thumb_set sub_08042154, CanDropAnyCargoAt\n");

/* The c_0804247C.c map-cell idiom plus a per-terrain movement-cost lookup:
 * gUnknown_085D5ABC[type].unk14 points at a blob whose +0x1a is a table of
 * cost bytes indexed by the cell's low 5 bits, and a zero entry means the
 * terrain is impassable for this unit type.
 *
 * `idx` and `costs` are SEPARATE statements and the order between them is
 * load-bearing. Writing the lookup as `t[0x1a + idx]` folds 0x1a into the
 * `ldrb` displacement; binding `costs = t + 0x1a` earlier emits the
 * `adds r4,#0x1a` before the cell load. Only cells -> idx -> costs puts the
 * add between the `ands` and the `ldrb`, where the ROM has it. */
bool8 CanDropFirstCargoAt(struct Unk030040D8 *a1, s16 a2, s16 a3)
{
    u8 *t;
    struct Map *p;
    u8 *rows;
    u8 *cells;
    u8 *costs;
    int y2;
    int off;
    int idx;

    t = gUnknown_085D5ABC[a1->unk00].transportTable;
    p = gMap;
    y2 = a3 * 2;
    rows = (u8 *)p->rowOffset;
    off = *(u16 *)(rows + y2) + a2;
    cells = p->terrain;
    idx = cells[off] & 0x1f;
    costs = t + 0x1a;

    if (costs[idx] == 0)
        return FALSE;

    if (GetDropDirectionMask(a2, a3, a1->unk07[0]) != 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_080421D0\n.thumb_set sub_080421D0, CanDropFirstCargoAt\n");

/* CanDropFirstCargoAt's twin, differing only in which of the two adjacent flag bytes
 * at +7/+8 it hands to GetDropDirectionMask. See c_080421D0.c for why `idx` and
 * `costs` are separate statements in that order. */
bool8 CanDropSecondCargoAt(struct Unk030040D8 *a1, s16 a2, s16 a3)
{
    u8 *t;
    struct Map *p;
    u8 *rows;
    u8 *cells;
    u8 *costs;
    int y2;
    int off;
    int idx;

    t = gUnknown_085D5ABC[a1->unk00].transportTable;
    p = gMap;
    y2 = a3 * 2;
    rows = (u8 *)p->rowOffset;
    off = *(u16 *)(rows + y2) + a2;
    cells = p->terrain;
    idx = cells[off] & 0x1f;
    costs = t + 0x1a;

    if (costs[idx] == 0)
        return FALSE;

    if (GetDropDirectionMask(a2, a3, a1->unk07[1]) != 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_0804223C\n.thumb_set sub_0804223C, CanDropSecondCargoAt\n");

/* Five values stay live across the two calls -- the address of
 * gUnknown_08499590, the 0x417A constant, y * 2, the narrowed x and the unit
 * pointer -- which is what the sl/sb/r8 saves are for, not a loop.
 *
 * The post-call half needs its OWN p/rows/off locals. Sharing them with the
 * pre-call half keeps p live past `ids`, and the ROM's single `adds r2,#0x12`
 * (which consumes p in place) then comes out as a copy plus an add.
 *
 * unit is bound BEFORE the `id == 0` test, the same ordering GetDropDirectionMask
 * uses, and the CanTransportCarry argument is respelled &gUnits[id]
 * rather than reusing unit: that global is a non-const pointer, so the first
 * `bl` kills its MEM and the ROM reloads it. */
bool8 IsBoardableTransportAt(s16 x, s16 y)
{
    struct Map *p;
    u8 *rows;
    u8 *ids;
    struct Map *p2;
    u8 *rows2;
    u8 *cells;
    u8 *costs;
    u8 *t;
    struct Unit *unit;
    int y2;
    int off;
    int off2;
    int id;
    int idx;

    p = gMap;
    y2 = y * 2;
    rows = (u8 *)p->rowOffset;
    off = *(u16 *)(rows + y2) + x;
    ids = p->unit;
    id = ids[off];
    unit = &gUnits[id];

    if (id == 0)
        return FALSE;

    if (AreUnitsOnSameTeam(id, gUnknown_03003F38) == 0)
        return FALSE;

    if (CanTransportCarry(&gUnits[id], gUnknown_030040D8->unk00) == 0)
        return FALSE;

    t = gUnknown_085D5ABC[unit->type].transportTable;

    p2 = gMap;
    rows2 = (u8 *)p2->rowOffset;
    off2 = *(u16 *)(rows2 + y2) + x;
    cells = p2->terrain;
    idx = cells[off2] & 0x1f;
    costs = t + 0x1a;

    if (costs[idx] == 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_080422A8\n.thumb_set sub_080422A8, IsBoardableTransportAt\n");

/* Reads the same cell TWICE around the IsTerrainOwnedByUnitsTeam call, and the second read
 * needs its own set of locals. Reusing p/rows/off/cells for both halves makes
 * each one pseudo whose live range spans the `bl`, which pushes the 0x1432
 * constant into a callee-saved register and adds r7 to the push list; the ROM
 * keeps it in scratch r0. Only y * 2 and the narrowed x survive the call, in
 * r5 and r4. */
bool8 IsCellCapturableByCurrentArmy(s16 x, s16 y)
{
    u8 army;
    struct Map *p;
    u8 *rows;
    u8 *cells;
    struct Map *p2;
    u8 *rows2;
    u8 *cells2;
    int y2;
    int off;
    int off2;

    army = gUnknown_03003F38;
    p = gMap;
    y2 = y * 2;
    rows = (u8 *)p->rowOffset;
    off = *(u16 *)(rows + y2) + x;
    cells = p->terrain;

    if (IsTerrainOwnedByUnitsTeam(army, cells[off]) == 1)
        return FALSE;

    p2 = gMap;
    rows2 = (u8 *)p2->rowOffset;
    off2 = *(u16 *)(rows2 + y2) + x;
    cells2 = p2->terrain;

    switch (cells2[off2] & 0x1f)
    {
    case 6:
    case 8:
    case 10:
    case 11:
    case 14:
    case 20:
        return TRUE;
    default:
        return FALSE;
    }
}
asm(".global sub_0804236C\n.thumb_set sub_0804236C, IsCellCapturableByCurrentArmy\n");
