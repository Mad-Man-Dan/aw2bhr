#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CBA0.
 * sub_0802CBA0 @ 0x0802CBA0, sub_0802CBC8 @ 0x0802CBC8, sub_0802CC04 @ 0x0802CC04
 */

/* The cursor-position predicate the whole 0x0802CB-0x0802CD cluster is built
 * on. UnitMenu_DiveUsability, UnitMenu_RiseUsability, UnitMenu_DropFirstUsability and UnitMenu_DropSecondUsability all call it.
 *
 * `movs r2,#0; ldrsh r0,[r1,r2]` and `movs r2,#2; ldrsh r1,[r1,r2]` is the
 * s16-OBJECT tell, and it is why union Unk802C57CBuf gained a signed view: the
 * u16 `.pos` spelling emits `ldrh` and lets IsBoardableTransportAt's declared s16
 * parameters do the sign extension, and an (s16) cast on a u16 member emits
 * `ldrh; lsls #16; asrs #16`. Only a genuinely s16 member produces the register-
 * offset `ldrsh` here (ldrsh has no immediate-offset form, hence the scratch).
 *
 * `lsls #0x18; lsrs #0x18; cmp #1` is IsBoardableTransportAt's bool8 return kept for a
 * compare against a non-zero constant. The THEN arm is `return FALSE` and it
 * sits after the pool, so this is the IntelMenu_RulesUsability spelling.
 */

bool8 UnitMenu_LoadUsability(void)
{
    if (IsBoardableTransportAt(gUnknown_03003100.spos.unk00, gUnknown_03003100.spos.unk02) == TRUE)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802CBA0\n.thumb_set sub_0802CBA0, UnitMenu_LoadUsability\n");

/* UnitMenu_LoadUsability gated in front of a second cursor test, in UnitMenu_RiseUsability's
 * negated-disjunction spelling (see that function for why the `&&` form does
 * not match).
 *
 * The coordinate pair goes through union Unk802C57CBuf's signed view for the
 * `ldrsh` pair, and CanDropFirstCargoAt's own prologue confirms the s16 parameters
 * (`lsls #0x10; asrs #0x10` on both) and the wide leading pointer (bare
 * `adds r7, r0, #0`, then `ldrb r4, [r7]` -- struct Unk030040D8's unk00).
 */

bool8 UnitMenu_DropFirstUsability(void)
{
    if (!UnitMenu_LoadUsability()
        || CanDropFirstCargoAt(gUnknown_030040D8, gUnknown_03003100.spos.unk00, gUnknown_03003100.spos.unk02) != TRUE)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802CBC8\n.thumb_set sub_0802CBC8, UnitMenu_DropFirstUsability\n");

/* UnitMenu_DropFirstUsability's twin: byte-identical apart from the callee, CanDropSecondCargoAt
 * instead of CanDropFirstCargoAt. The two callees are themselves near-duplicates --
 * same prologue, same 0x5c-stride gUnknown_085D5ABC lookup, differing only in
 * which byte of the leading pointer they forward (+7 vs +8).
 */

bool8 UnitMenu_DropSecondUsability(void)
{
    if (!UnitMenu_LoadUsability()
        || CanDropSecondCargoAt(gUnknown_030040D8, gUnknown_03003100.spos.unk00, gUnknown_03003100.spos.unk02) != TRUE)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802CC04\n.thumb_set sub_0802CC04, UnitMenu_DropSecondUsability\n");
