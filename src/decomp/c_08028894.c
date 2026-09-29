#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028894.
 * sub_08028894 @ 0x08028894, sub_080288D8 @ 0x080288D8, sub_08028904 @ 0x08028904
 */

/* BOTH PARAMETERS ARE `int`. `adds r4,r0,#0` / `adds r5,r1,#0` followed by
 * SEPARATELY narrowed copies for StartArmyDefeat's two u16 parameters, and then
 * the RAW originals for RecordArmyDefeat, is the copy-then-narrow tell: the
 * narrowing is a cast at a use, not the parameter's width. `cmp r0, r4` against
 * the u16 gUnknown_030033EC compares the raw value and corroborates it.
 *
 * That second call is also what retyped RecordArmyDefeat's second parameter from
 * `u8` to `int` in this wave -- see its declaration. */
void DefeatArmy(int a1, int a2)
{
    StartArmyDefeat(a1, a2);
    RecordArmyDefeat(a1, a2);

    if (gUnknown_030033EC == a1)
    {
        gUnknown_030032D8 = 1;
        ReadyCurrentArmyUnits();
    }

    if (a2 == 2)
        ParkMapState();
}
asm(".global sub_08028894\n.thumb_set sub_08028894, DefeatArmy\n");

/* u16 parameter (`lsls #0x10; lsrs #0x10` in place at entry, PROMOTE_MODE),
 * narrowed again to a byte for IsPlayerAliveAndActive and compared whole against the u8
 * gUnknown_030030F8. */
u8 sub_080288D8(u16 a1)
{
    if (IsPlayerAliveAndActive(a1) && gUnknown_030030F8 != a1)
        return 1;
    else
        return 0;
}

/* `lsls r0,#4; subs r0,r0,r4; lsls r0,#2` is a MULTIPLY by 0x3c, i.e. the
 * stride of gPlayers's elements -- written as a subscript, not as
 * hand-rolled arithmetic. gPlayers is a POINTER variable in ROM, so
 * the base arrives through `ldr r1, [r0]`.
 *
 * BOTH FAILING PATHS RETURN 0 AND THE SUCCESS IS THE LAST STATEMENT. The ROM
 * puts `movs r0, #0` at the fall-through of the final test and `movs r0, #1`
 * past the literal pool, reached only by branches. Every spelling that ends on
 * `return 0` -- including `if (f(a) && (A || B)) return 1; return 0;` -- swaps
 * the two blocks, because gcc lays the last-written return out last. */
u8 CheckArmySurvivesTeamRule(u16 a1)
{
    if (IsPlayerAliveAndActive(a1) == 0)
        return 0;

    if (gPlayers[a1].defeated == 0 && gPlayers[a1].unk31 != 0)
        return 0;

    return 1;
}
asm(".global sub_08028904\n.thumb_set sub_08028904, CheckArmySurvivesTeamRule\n");
