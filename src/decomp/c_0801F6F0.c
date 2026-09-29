#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F6F0.
 * sub_0801F6F0 @ 0x0801F6F0
 */

/* One neighbour step of the movement-range flood fill. Adds the cursor origin
 * gUnknown_0300409C to the (dx, dy) it is given, costs the destination cell,
 * and -- if the new cost beats what gUnknown_03003340 already holds and the
 * cell passes the four bounds/blocking tests -- writes a 4-byte {x, y, tag,
 * cost} record through the gUnknown_03003F64 queue cursor and records the cost.
 *
 * `idx` IS ASSIGNED INSIDE THE EXPRESSION, and that is the whole ordering fact.
 * As its own preceding statement it loads gUnknown_08499590 first; the ROM
 * loads gUnknown_084999C8 first, because that pointer is the OUTERMOST base of
 * the cost term and the map is only reached inside its index. Written as a
 * separate statement everything else is identical and the two pool words come
 * out swapped.
 *
 * The `&` operands are `unk2a & unk24[...]`, not the other way round: the ROM
 * materialises `&p->unk2a` (three instructions, since 0x2a is past the `ldrb`
 * displacement range) BEFORE the `lsrs #0x1e` that indexes unk24.
 *
 * `x` and `y` are u8 LOCALS -- shorten_compare narrows both bound tests to
 * unsigned char, which is where the `bhs` comes from; an int local gives the
 * signed `bge`. `v` is u16 and the two views of the same sum (`lsrs #0x10` for
 * the stored byte, `asrs #0x10` for the two signed compares) fall out of that
 * one declaration. */

void MapFloodCoreStep(u8 a1, u8 a2, u8 a3)
{
    u8 x;
    u8 y;
    int idx;
    u16 v;

    x = a2 + gUnknown_0300409C->unk00;
    y = a3 + gUnknown_0300409C->unk01;

    v = gUnknown_084999C8->unk00[gMap->terrain[
            idx = gMap->rowOffset[y] + x] & 0x1F]
        + (s8)gUnknown_03003340[gUnknown_0300409C->unk01][gUnknown_0300409C->unk00];

    if ((s16)v >= gUnknown_03003340[y][x])
        return;

    if (gUnknown_084999C8->unk22 != 0
        && gMap->unit[idx] != 0
        && (gUnknown_084999C8->unk2a
            & gUnknown_084999C8->unk24[gMap->unit[idx] >> 6]))
        return;

    if ((s16)v > gUnknown_084999C8->unk20)
        return;
    if (x >= gUnknown_084999C8->unk28)
        return;
    if (y >= gUnknown_084999C8->unk29)
        return;
    if (gUnknown_030040E0 > 0x15C)
        return;

    gUnknown_030040E0++;
    gUnknown_03003F64[0] = x;
    gUnknown_03003F64[1] = y;
    gUnknown_03003F64[2] = a1;
    gUnknown_03003F64[3] = v;
    gUnknown_03003F64 += 4;
    gUnknown_03003340[y][x] = v;
}
asm(".global sub_0801F6F0\n.thumb_set sub_0801F6F0, MapFloodCoreStep\n");
