#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020EDC.
 * AddValueInRange @ 0x08020EDC
 *
 * Not a Xenesis-documented name. The old sub_08020EDC symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "map.h"

/* Adds `delta` to every cell of the map-sized u8 plane `buf` within Manhattan
 * distance r of (x, y). r == 0 touches only the centre cell. Beyond distance
 * 1, when bit 3 of sub_08043050(flags) is clear, a cell of terrain type 4 or
 * 0x13 is skipped if it holds no unit or a unit whose type is outside
 * 0x10..0x14. sub_080210C8 calls it with planes in gMap->visible. Twin of
 * sub_08020B88, which writes an overlay instead of adding.
 *
 * Measured spelling notes (parked since wave 49 at 90.7%):
 * - `flags` is an INT parameter. The one caller, sub_080210C8, narrows its
 *   own int with an explicit `(u8)` at the call; a u8 parameter here would
 *   narrow it a second time at entry, and that half-emitted narrowing is
 *   what slipped `lsls r4,#24` ahead of delta's group. Earlier waves read the
 *   caller's `lsls #0x18; lsrs #0x18` as proof of a u8 prototype. It is the
 *   cast.
 * - `d` is `u32 d = (u8)delta;`, a word-wide copy. As a u8 local the
 *   `buf[...] += d` adds are emitted with their operands swapped.
 * - Map access through gMap's members (rowOffset, terrain, unitUnk, width,
 *   height) rather than byte offsets. The centre cell is
 *   `buf[gMap->rowOffset[y] + x]`, row first, and the swept cell reads its
 *   row into a u16 `row` before the add.
 */
void AddValueInRange(s16 x, s16 y, s16 r, u8 *buf, int delta, int flags)
{
    u32 d;
    u8 f;
    s16 xx;
    s16 yy;
    s16 dy;
    int off;
    int t;
    u16 row;
    struct Unit *unit;

    d = (u8)delta;
    f = flags;

    if (r == 0)
    {
        buf[gMap->rowOffset[y] + x] += d;
        return;
    }

    dy = -r;

    for (yy = y - r; yy <= y + r; yy++, dy++)
    {
        if (yy < 0)
            continue;
        if (yy >= gMap->height)
            continue;

        for (xx = x - r + (dy < 0 ? -dy : dy);
             xx <= x + r - (dy < 0 ? -dy : dy);
             xx++)
        {
            if (xx < 0)
                continue;
            if (xx >= gMap->width)
                continue;

            if ((sub_08043050(f) & 8) == 0
             && (xx - x < 0 ? x - xx : xx - x)
              + (yy - y < 0 ? y - yy : yy - y) > 1)
            {
                off = xx + gMap->rowOffset[yy];
                t = gMap->terrain[off] & 0x1f;
                if (t == 4 || t == 0x13)
                {
                    if (gMap->unitUnk[off] == 0)
                        continue;
                    if ((u8)((unit = &gUnits[gMap->unitUnk[off]])->type - 0x10) > 4)
                        continue;
                }
            }

            row = gMap->rowOffset[yy];
            buf[xx + row] += d;
        }
    }
}

asm(".global sub_08020EDC\n.thumb_set sub_08020EDC, AddValueInRange\n");
