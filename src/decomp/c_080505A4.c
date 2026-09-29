#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080505A4.
 * sub_080505A4 @ 0x080505A4
 */

/* The two levers, both measured:
 *   - The gUnknown_085D6A48 ROW must be a STRUCT. `gUnknown_085D6A48[j][10]` on
 *     the declared `u16 [][12]` emits `adds rB,#0x14; ldrh [rB]` where the ROM
 *     keeps the offset in the load displacement.
 *   - The COMMA ANCHOR fixes the pool order. The ROM loads gUnknown_085D6A48's
 *     address BEFORE gUnknown_03004580's; the flat subscript loads them the
 *     other way round because the subscript is evaluated first. Assigning the
 *     table base inside the condition, ahead of the subscript, reverses it
 *     without moving anything else -- see docs/agbcc-codegen.md.
 *   - `t` is a u16 local computed before the first `if` (hence the u16
 *     truncation the ROM has there); the second `* 7 + 5` is an `int` local
 *     declared inside the second `if`, which is what puts it ahead of the two
 *     pool loads and gives it no truncation of its own. */
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u8 filler_00[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 filler_16[0x02];
};

void SpawnThirdEffectAndProjectile(u16 a1, u16 a2)
{
    struct Unk85D6A48Row *tbl;
    u16 t;

    gUnknown_0300453C = a1;
    gUnknown_0300451C = a2;

    t = gUnknown_08552178[a1][a2] * 7 + 2;

    if (gUnknown_020296B0[a1].unk00 != 0)
        gUnknown_020296B0[a1].unk02[a2] =
            sub_08015410(gUnknown_08553628, 1, gUnknown_02029700[a1][0],
                         gUnknown_02029700[a1][1], t);

    if ((tbl = (struct Unk85D6A48Row *)gUnknown_085D6A48,
         tbl[gUnknown_03004580[a1][1]].unk14) != 0
        && gUnknown_03004580[a1][2] == 1)
    {
        int n;

        n = gUnknown_08552178[a1][a2] * 7 + 5;
        gUnknown_02029710[a1].unk0c[a2] =
            sub_08015410(gUnknown_08553610, 1, gUnknown_02029710[a1].unk04,
                         gUnknown_02029710[a1].unk08, n);
    }
}
asm(".global sub_080505A4\n.thumb_set sub_080505A4, SpawnThirdEffectAndProjectile\n");
