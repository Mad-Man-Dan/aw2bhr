#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805C128.
 * sub_0805C128 @ 0x0805C128
 */

/* WAVE 35 canonical `struct Map`, extended by W36-D with +0x1432 (carved out of
 * unitUnk's filler; byte-neutral, since only a field's START OFFSET enters the
 * address arithmetic). Keep every draft in this unit on the same body. */

/* MATCHED (wave 52, W52-C) -- 152/152 bytes, relocs match.
 *
 * The 0x0805C block's own accept-a-cell predicate, the same skeleton as
 * AiCheckDropNeighbour with a different filter: the cell must be EMPTY (+0x12 is 0
 * rather than "mine or neutral"), its gUnknown_03003340 byte non-negative, and
 * its terrain code neither 0x0b nor 0x0d. Cells whose gUnknown_085767D5 entry
 * is set must additionally carry gUnknown_03004084 in the top three bits of the
 * same +0x1432 byte.
 *
 * WHAT CLOSED IT, after two waves at 98.7% (2 of 152 bytes, the AND's operand
 * tie):
 *
 *     original   movs r0,#0xe0 / ldr r1,=g / ands r0, r3 / ldrh / cmp r0, r1
 *     old draft  movs r0,#0xe0 / ldr r1,=g / ands r3, r0 / ldrh / cmp r3, r1
 *
 * THE FIX IS TO DELETE THE `u8 cell` LOCAL AND NAME `map->terrain[idx]` TWICE.
 * The old draft recorded "`cell` is `u8`, not `int`. The ROM holds ONE QImode
 * pseudo live across both masks" as SETTLED. That was exactly backwards and it
 * is what cost this function two waves. Reading the member twice still emits
 * ONE `ldrb` -- CSE merges the loads -- but the two masks are then separate
 * tree nodes over a value with two REFERENCES, and reload's commutative `%`
 * swap on `*thumb_andsi3` ties the destination to the CONSTANT's register
 * instead of to the byte's. Binding the local makes the second mask the last
 * use of a one-reference pseudo, which flips the tie. Same instruction count,
 * same schedule; the binding is worth 2 bytes and nothing else.
 *
 * Corroborated independently in the same batch: AiListThreatenedProperties has the identical
 * `& 0x1f` / `& 0xe0` pair on the same plane byte and shows the same flip --
 * naming the member twice gives `ands r0, r1` (the ROM), binding it to a `u8`
 * local gives `ands r1, r0`. Two functions, same lever, opposite of the note
 * that had been sitting here.
 *
 * The constant-first spelling `0xe0 & map->terrain[idx]` is still required and
 * is unchanged from the 98.7% draft: agbcc emits a comparison's operand classes
 * in source order, so this schedules the pool `ldr` for gUnknown_03004084
 * between the `movs #0xe0` and the `ands`, which is the ROM's interleaving.
 *
 * ALSO RULED OUT this wave: `int cell` holding the raw byte with a second
 * narrow local for the 5-bit code -- the old draft's "first thing a next
 * attempt should try". It scores 96.1%, worse than the draft it replaced: the
 * SImode pseudo sinks the pool `ldr` below the `ands` as well as keeping the
 * wrong tie. */
void sub_0805C128(int x, int y, u16 * out)
{
    struct Map * map;
    int idx;
    int t;

    if (x < 0)
        return;
    if (y < 0)
        return;

    map = gMap;

    if (x >= map->width)
        return;
    if (y >= map->height)
        return;

    idx = map->rowOffset[y] + x;

    if (gMap->unit[idx] != 0)
        return;

    if ((s8)gUnknown_03003340[y][x] < 0)
        return;

    t = map->terrain[idx] & 0x1f;

    if (t == 0xd)
        return;
    if (t == 0xb)
        return;

    if (gUnknown_085767D5[t] != 0
        && (0xe0 & map->terrain[idx]) != gUnknown_03004084)
        return;

    out[0] = x;
    out[1] = y;
}
