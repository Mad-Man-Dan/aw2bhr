#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038C98.
 * sub_08038C98 @ 0x08038C98
 */

/* Opens the move-range display for the unit gUnknown_030040D8 points at: loads
 * the cursor palette and tiles, seeds the fuel budget with the smaller of the
 * unit's remaining fuel (unk06_0) and its movement allowance, then pushes the
 * unit's own cell as step 0 of the gUnknown_0849D5F8 stack.
 *
 * gUnknown_030040D8 is a `struct Unit *` in disguise -- the pointer
 * SUBTRACTION against gUnits proves it, since the 0x0c-byte
 * reciprocal multiply only comes out of a 12-byte pointee. The declaration
 * keeps its own struct tag, so every use here casts; include/unknown-globals.h
 * already records the same conflation for gUnknown_08091364.
 * `((p - gUnits) >> 6) + 1` is src/decomp/c_080211DC.c's army-index
 * idiom verbatim.
 *
 * The two arms are a plain if/else with a STORE IN EACH, not a `?:` -- both
 * recompute their value and cross-jumping merged the `strb` afterwards, which
 * is why the ROM has the GetUnitMovementWithCoBonus call twice. */
void InitMovePathForActiveUnit(void)
{
    ApplyPaletteExt(gUnknown_0809165C, 0x260, 0x20);
    RegisterDataMove(gUnknown_080A1C24, (void *)0x06013940, 0xa00);

    if (((struct Unit *)gUnknown_030040D8)->fuel
        < GetUnitMovementWithCoBonus(((((struct Unit *)gUnknown_030040D8)
                         - gUnits) >> 6) + 1,
                       ((struct Unit *)gUnknown_030040D8)->type))
        gUnknown_0849D5F8->unk38[0xc]
            = ((struct Unit *)gUnknown_030040D8)->fuel;
    else
        gUnknown_0849D5F8->unk38[0xc]
            = GetUnitMovementWithCoBonus(((((struct Unit *)gUnknown_030040D8)
                             - gUnits) >> 6) + 1,
                           ((struct Unit *)gUnknown_030040D8)->type);

    TruncateMovePath(0);
    PushMovePathStep(((struct Unit *)gUnknown_030040D8)->x,
                 ((struct Unit *)gUnknown_030040D8)->y);
    gUnknown_0849D5F8->unk38[0] = gUnknown_0849D5F8->unk38[0xc];
    SetMovePathLastCursor(0xFFFF, 0xFFFF);
    UpdateMovePathToCursor();
}
asm(".global sub_08038C98\n.thumb_set sub_08038C98, InitMovePathForActiveUnit\n");
