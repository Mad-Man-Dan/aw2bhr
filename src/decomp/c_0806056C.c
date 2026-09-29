#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806056C.
 * sub_0806056C @ 0x0806056C
 */

/* AiExecutorStartCargoDrop @ 0x0806056C, 280 bytes.
 *
 * The byte at gUnknown_030046C0 + 6 + a1 has to be reached through a POINTER
 * LOCAL that is advanced by 6 in a statement of its own. Every spelling that
 * names the member -- `(&gUnknown_030046C0.unk06)[a1]`, `((u8 *)&g)[a1 + 6]`,
 * a member array at that offset -- either folds the 6 into the literal pool
 * word (`.word gUnknown_030046C0+0x6`) or into the `ldrb` displacement. The ROM
 * has the BARE symbol in the pool and a separate `adds r0, #6`, which is what
 * -fforce-addr's force_reg of the ADDR_EXPR plus a later plus_constant on the
 * register produces, and only the two-statement pointer form gets there.
 *
 * The `(((struct Unit *)gUnknown_030040D8 - gUnits) & 0xc0)
 * >> 6` army index is copied from the matched c_0802966C.c, which makes the
 * same CreateMoveSlideWithPath call.
 */
void AiExecutorStartCargoDrop(u8 a1)
{
    struct Unit *unit;
    u8 *p;

    gUnknown_030045E0[a1] = 0;

    p = (u8 *)&gUnknown_030046C0;
    p += 6;

    if (p[a1] == 5)
    {
        unit = &gUnits[gUnknown_030040D8->unk07[a1]];
        gUnknown_03003110[0] = 0xa;
        gUnknown_03003110[1] = 4;
        gUnknown_030045E0[a1] = (struct Unk35828Proc *)CreateMoveSlideWithPath(
            gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
            (((struct Unit *)gUnknown_030040D8 - gUnits) & 0xc0) >> 6,
            unit->type, gUnknown_03003110);
    }
    else if (p[a1] != 0)
    {
        gUnknown_03003F40 = p[a1] - 1;
        unit = &gUnits[gUnknown_030040D8->unk07[a1]];
        gUnknown_030045E0[a1] = (struct Unk35828Proc *)CreateMoveSlideWithPath(
            gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
            (((struct Unit *)gUnknown_030040D8 - gUnits) & 0xc0) >> 6,
            unit->type, &gUnknown_08576900[gUnknown_03003F40]);
        DropCargoUnit(a1);
        gUnknown_03004774++;
        sub_08029868(unit->type);
    }
}
asm(".global sub_0806056C\n.thumb_set sub_0806056C, AiExecutorStartCargoDrop\n");
