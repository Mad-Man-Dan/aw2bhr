#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805CA24.
 * sub_0805CA24 @ 0x0805CA24
 */

/* `movs r0, #0x5c; muls r0, r1, r0` is the 0x5c stride of struct UnitType
 * -- agbcc does not strength-reduce it -- so this is `g[type]`, the unit-type
 * record for whatever gUnknown_030040D8 currently points at.
 *
 * `ldrh r1, [r3, #4]` is a HALFWORD read of the 0x04 bitfield container whose
 * high byte src/decomp/c_080424BC.c already spells as `unk05 &= 7`. The mask
 * 0x780 (`movs #0xf0; lsls #3`) spans bits 7..10, i.e. the top bit of the byte
 * at +0x04 and the low three of unk05, so it crosses the byte boundary and no
 * byte member can express it. The cast is the layout-preserving way to say
 * that until struct Unk030040D8 and struct Unit are merged.
 *
 * The predicate is written the NEGATIVE way round -- `return 0` under a
 * disjunction, `return 1` as the bare trailing statement -- and that is forced,
 * not stylistic. The positive spelling
 * `if (a != 0 && (b == 0 || c != 0)) return 1; return 0;` computes the same
 * three tests in the same order and lays the two constants out in the opposite
 * places: `movs r0,#1` inline and `movs r0,#0` past the pool. 5 of 60 bytes.
 * The `== 0` on the leading test is what puts the `return 0` block inline,
 * because do_jump rewrites a comparison against zero by exchanging its
 * labels. */

int AiUnitHasUsableWeapon(void)
{
    struct UnitType *type = &gUnknown_085D5ABC[gUnknown_030040D8->unk00];

    if ((type->unk11 | type->minRange) == 0
        || (type->maxAmmo != 0
            && (*(u16 *)((u8 *)gUnknown_030040D8 + 4) & 0x780) == 0))
        return 0;

    return 1;
}
asm(".global sub_0805CA24\n.thumb_set sub_0805CA24, AiUnitHasUsableWeapon\n");
