#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E3D8.
 * sub_0803E3D8 @ 0x0803E3D8
 */

/* MATCHED (wave 49, W49-C), first attempt. 380/380 bytes.
 *
 * PROMOTION NOTE: the only reloc difference is agbcc's own -fforce-addr copy of
 * &gUnknown_08499590, which asm/ prints as `gUnknown_080912F8` because gen_lds.py
 * invents a symbol for the ROM word. The ROM word at 0x080912F8 contains
 * 0x08499590. Do NOT declare gUnknown_080912F8. This entry needs
 *   "rodata": ["0x080912F8"]
 * in data/promoted.json, then tools/split_rodata.py + tools/gen_lds.py.
 *
 * The map planes are reached as MEMBERS of a struct cast onto gUnknown_08499590
 * (W34-F's rule): a flat `gUnknown_08499590[0x1432 + rowOffset[y] + x]`
 * reassociates to `(p + idx) + K` where the ROM has `(p + K) + idx`. Both the
 * 0x417A row table and the 0x1432 terrain plane need it here, and the 0x417A
 * constant is too wide for `adds #imm8`, which is why the ROM spends a register
 * and a pool word on it -- that is a consequence of the offset, not a lever.
 *
 * The switch is the dense `subs #0x15; cmp #0xa; bhi` form, so case N of the
 * 11-entry jump table at _0803E464 is source value N + 0x15. Case BODIES come
 * out in SOURCE order, which is what fixes the odd-looking order below
 * (0x19, then 0x16/0x15/0x17/0x18, then 0x1c/0x1b/0x1a/0x1d/0x1f/0x1e); the
 * four AddMinicannonInventionRecord arms are cross-jumped onto one shared tail by jump.c, so
 * they are four ordinary cases differing only in the fifth argument, not a
 * grouped case label.
 */

#define MAP gMap

void SpawnInventionRecords(void)
{
    int x;
    int y;
    int a;
    int b;

    a = gUnknown_085C77A0[gPlaySt.mapID].unk1c;
    b = gUnknown_085C77A0[gPlaySt.mapID].unk1e;
    ClearInventionRecords();
    for (y = 0; y < MAP->height; y++)
    {
        for (x = 0; x < MAP->width; x++)
        {
            if (FindInventionAt(x, y))
                continue;
            switch (MAP->terrain[MAP->rowOffset[y] + x] & 0x1f)
            {
            case TERRAIN_LASER:
                AddLaserInventionRecord(x, y, a, b);
                break;
            case TERRAIN_MINICANNON_N:
                AddMinicannonInventionRecord(x, y, a, b, 1);
                break;
            case TERRAIN_MINICANNON_S:
                AddMinicannonInventionRecord(x, y, a, b, 0);
                break;
            case TERRAIN_MINICANNON_W:
                AddMinicannonInventionRecord(x, y, a, b, 2);
                break;
            case TERRAIN_MINICANNON_E:
                AddMinicannonInventionRecord(x, y, a, b, 3);
                break;
            case TERRAIN_VOLCANO:
                AddVolcanoInventionRecord(x, y, 4, 4, 2, 1);
                break;
            case TERRAIN_CANNON_N:
                AddCannonInventionRecord(x, y, 3, 3, a, 2, 1);
                break;
            case TERRAIN_CANNON_S:
                AddCannonInventionRecord(x, y, 3, 3, a, b, 0);
                break;
            case TERRAIN_FACTORY:
                AddFactoryInventionRecord(x, y, 3, 4, 1, 1);
                break;
            case TERRAIN_BLOCKED:
                AddBlockedInventionRecord(x, y, 4, 4);
                break;
            case TERRAIN_DEATHRAY:
                AddDeathRayInventionRecord(x, y, 3, 3, 7, 7);
                break;
            }
        }
    }
}
asm(".global sub_0803E3D8\n.thumb_set sub_0803E3D8, SpawnInventionRecords\n");
