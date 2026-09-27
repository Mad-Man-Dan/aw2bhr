#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08042998.
 * JoinUnits @ 0x08042998
 *
 * Not a Xenesis-documented name. The old sub_08042998 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "map.h"
/* gUnknown_030040D8 points at the selected unit (sub_0802E4B4 sets it to
 * &gUnits[...]); the header types it as a layout-only mirror struct. */
#define gSelectedUnit ((struct Unit *)gUnknown_030040D8)

/* Joins the unit under the cursor into the selected unit. HP adds up in
 * display units (1..10 each), and anything over 10 is paid back to the current
 * player (sub_08025B58) at sub_08042C9C's per-HP value for the unit type. The
 * joiner's unk05_3 is kept, the higher of the two unk06_7 flags wins, and ammo
 * and fuel add up, capped at the type's maxAmmo / maxFuel. Then the joiner's
 * slot is freed (type 0).
 *
 * Measured spelling notes (14.2% / -28 bytes -> match):
 * - The sums are narrow: ONE `u8 sum` is reused for the ammo total and then
 *   the fuel total. u8 gives the ROM's unsigned `bls` compares and the
 *   un-merged per-branch bitfield stores (the "un-cross-jumped" residual of
 *   the old draft). A u32 sum is 12 bytes short; separate u8 locals match
 *   the size but swap two registers in the fuel block (98.1%). The shared
 *   local is what decomp-permuter found.
 * - The HP total is a conditional expression: the ROM computes both arms
 *   into one temporary and copies it into totalHp after the join. An if/else
 *   assigns it in each arm (35%).
 * - gUnknown_030040D8 is named directly. -fforce-addr then emits the ROM's
 *   pool word 0x08091364 and the three-load chain. The old draft spelled that
 *   word as a pointer-to-pointer local.
 */
void JoinUnits(void)
{
    struct Unit *joiner;
    int joinerHp;
    u8 totalHp;
    u8 sum;
    u16 player;

    joiner = &gUnits[gMap->unitUnk[gMap->rowOffset[gUnknown_03003100.pos.unk02]
                                   + gUnknown_03003100.pos.unk00]];

    if (joiner->hp != 0)
        joinerHp = Div(joiner->hp - 1, 10) + 1;
    else
        joinerHp = 0;

    totalHp = gSelectedUnit->hp != 0
            ? joinerHp + 1 + Div(gSelectedUnit->hp - 1, 10)
            : joinerHp;

    if (totalHp > 10)
    {
        player = gUnknown_030033EC;
        sub_08025B58(player, sub_08042C9C(player, gSelectedUnit->type) * (totalHp - 10));
        totalHp = 10;
    }

    gSelectedUnit->hp = totalHp * 10;
    gSelectedUnit->unk05_3 = joiner->unk05_3;
    if (gSelectedUnit->unk06_7 < joiner->unk06_7)
        gSelectedUnit->unk06_7 = joiner->unk06_7;

    sum = gSelectedUnit->ammo + joiner->ammo;
    if (sum > gUnknown_085D5ABC[gSelectedUnit->type].maxAmmo)
        gSelectedUnit->ammo = gUnknown_085D5ABC[gSelectedUnit->type].maxAmmo;
    else
        gSelectedUnit->ammo = sum;

    if (gPlaySt.savingEnabled == 0 && gPlaySt.fog == 0)
    {
        sub_08025B80(gSelectedUnit, gUnknown_03004074);
        gUnknown_03004074 = 0;
    }

    sum = gSelectedUnit->fuel + joiner->fuel;
    if (sum > gUnknown_085D5ABC[gSelectedUnit->type].maxFuel)
        gSelectedUnit->fuel = gUnknown_085D5ABC[gSelectedUnit->type].maxFuel;
    else
        gSelectedUnit->fuel = sum;

    joiner->type = 0;
    sub_080424E4();
}

asm(".global sub_08042998\n.thumb_set sub_08042998, JoinUnits\n");
