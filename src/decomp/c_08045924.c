#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045924.
 * sub_08045924 @ 0x08045924
 */

/* "Does the current army have no unit left with a non-zero unk06_0?"  It walks
 * the army's 0x32 gUnits slots from `(u16)gUnknown_084995FE[army] + 1`
 * and counts the live ones whose unk06_0 bitfield is set, returning TRUE only
 * when the count is zero.
 *
 * gUnknown_084995FE is declared `const s16 []` but is really u16 -- the ROM
 * reads it with a bare `ldrh`, so the source has the `(u16)` cast; without it
 * agbcc emits `movs r1,#0; ldrsh`.  Same finding as work/CoPowerDamageHeal_DamageLoopSimple and
 * work/ArmyDefeat_Loop.
 *
 * The bound is NAMED TWICE (init and test) and CSEs to one `ldrh` chain; with no
 * call in the body everything is loop-invariant, so check_dbra_loop reverses the
 * ascending counter into `subs r3,#1 / bne` and strength_reduce turns the element
 * address into the r2 pointer giv.  The ascending source is what the bottom `bne`
 * reads out.
 *
 * `if (count == 0) return TRUE; return FALSE;` and not the inverse: agbcc
 * branches on the condition TRUE to the far arm and falls through to the
 * trailing return, so the constant the `beq` REACHES (1) is the `if` body. */
bool8 MapEventCond_CurrentArmyHasNoUnitWithFuel(void)
{
    int i;
    int count;

    count = 0;

    for (i = (u16)gUnknown_084995FE[gUnknown_03003F2C] + 1;
         i < (u16)gUnknown_084995FE[gUnknown_03003F2C] + 0x33; i++)
    {
        if (gUnits[i].type != 0 && gUnits[i].fuel != 0)
            count++;
    }

    if (count == 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_08045924\n.thumb_set sub_08045924, MapEventCond_CurrentArmyHasNoUnitWithFuel\n");
