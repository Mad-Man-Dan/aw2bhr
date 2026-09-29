#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ECE8.
 * sub_0803ECE8 @ 0x0803ECE8
 */

struct Unk803ECE8
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
};

/* `unit` is declared BEFORE `rec` and spells its index out in full rather than
 * through `rec`, and that ordering is the whole match. -fforce-addr materialises
 * &gUnits when `unit`'s initialiser is expanded, so the pool `ldr`
 * lands ahead of the index computation and is live across it -- which is why
 * the ROM has to spend r4 on the `ldrsh` zero index. Deriving `unit` from `rec`
 * instead (the obvious order) puts that pool `ldr` six instructions later and
 * frees r1 for the zero: size-exact, 86.1%, 15 bytes of pure register naming.
 * `rec` is CSE'd back onto the same expression, so it costs nothing. */
void InventionFire_ApplyDamage(struct Unk803ECE8 *p)
{
    struct Unk03003338 *tbl = gUnknown_03003338;
    struct Unit *unit = &gUnits[tbl[p->unk4c].unk00];
    struct Unk03003338 *rec = &tbl[p->unk4c];

    if (unit->hp - rec->unk02 > 0)
        unit->hp = unit->hp - rec->unk02;
    else
        unit->hp = 1;

    p->unk4c++;
    RebuildMapUnitLayers2();
}
asm(".global sub_0803ECE8\n.thumb_set sub_0803ECE8, InventionFire_ApplyDamage\n");
