#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080428F0.
 * sub_080428F0 @ 0x080428F0
 */

/* gUnknown_08091360 is NOT a global: it is a `-fforce-addr` address-constant
 * word inside the documented 0x08091350-0x0809138C pool block, and it holds
 * &gUnknown_030040D8. Naming the global directly is the honest spelling and
 * lets the build place the pool word.
 *
 * Both sums load their ROM table entry BEFORE the gUnknown_03003100 half, so
 * the cursor position is the FIRST addend in the source -- writing the table
 * first reverses both `ldrb` pairs and costs gUnknown_03003100 its register
 * priority, since the pool `ldr`s come out in first-reference order.
 *
 * gUnknown_0849FFF8 is s16 and gUnknown_03003100's members are s16, yet every
 * read here is `ldrb`: the sum is stored with `strb`, so combine narrows both
 * loads. The byte access is not evidence of a byte type. */
void DropCargoUnit(s16 a1)
{
    struct Unit *e;

    e = &gUnits[gUnknown_030040D8->unk07[a1]];
    e->flags &= ~8;
    e->flags &= ~2;
    e->x = gUnknown_03003100.pos.unk00 + gUnknown_0849FFF8[gUnknown_03003F40 * 2];
    e->y = gUnknown_03003100.pos.unk02 + gUnknown_0849FFF8[gUnknown_03003F40 * 2 + 1];
    gUnknown_030040D8->unk07[a1] = 0;
    if (a1 == 0 && gUnknown_030040D8->unk07[1] != 0)
    {
        gUnknown_030040D8->unk07[0] = gUnknown_030040D8->unk07[1];
        gUnknown_030040D8->unk07[1] = 0;
    }
    if (gUnknown_030040D8->unk07[0] == 0)
        gUnknown_030040D8->unk01 &= ~0x10;
}
asm(".global sub_080428F0\n.thumb_set sub_080428F0, DropCargoUnit\n");
