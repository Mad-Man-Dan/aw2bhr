#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804769C.
 * sub_0804769C @ 0x0804769C
 */

/* MATCHED byte-for-byte (wave 48, W48-I). No .rodata word needed.
 *
 * Counts how many of the p->unk21 slot indices listed in gUnknown_02028DD8
 * name a record whose unk07 or unk08 cargo id is `idx`.
 *
 * ONE COUNTER, ONE RETURN POINT. Writing the two early exits as `return count;`
 * lets agbcc const-propagate them into `movs r0,#0` / `movs r0,#1` past a
 * branch and costs 4 bytes; nesting them keeps the u16 counter in ip the whole
 * way, which is what the ROM does. The `adds #1; lsls #0x10; lsrs #0x10` on
 * every increment is what makes it u16 rather than int.
 *
 * gUnknown_03003F2C is the army base index for BOTH the 12-byte-stride array
 * subscript and the two `unk07 - base` / `unk08 - base` subtractions -- one
 * base, two distinct fields. The bound `p->unk21` is a u8 compared `bhs`, i.e.
 * UNSIGNED, and the loop counter is u16-narrowed.
 *
 * `struct Unk0804769C` lives in include/unknown-globals.h rather than here
 * because UnitList_Loop (matched this wave) passes the same record and the two
 * promote into different translation units.
 */

u16 sub_0804769C(struct Unk0804769C *p, u16 idx)
{
    u16 count;
    u16 i;
    struct Unit *r;

    count = 0;
    r = &gUnits[gUnknown_03003F2C + idx];
    if (r->flags & 8)
    {
        count = 1;
        if (!(r->flags & 0x10))
        {
            for (i = 0; i < p->unk21; i++)
            {
                r = &gUnits[gUnknown_03003F2C + gUnknown_02028DD8[i]];
                if (r->flags & 8)
                {
                    if (idx == r->unk07 - gUnknown_03003F2C
                     || idx == r->unk08 - gUnknown_03003F2C)
                        count = count + 1;
                }
            }
        }
    }

    return count;
}
