#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804096C.
 * sub_0804096C @ 0x0804096C
 */

/* Family F072, second member -- see RecountPropertiesIncomeAndAiFacilities. The bracketing here is
 * legible: BackupUnitStartPosition saves gUnknown_03003F24.pos into gUnknown_030044A4 and
 * RestoreUnitStartPosition puts it back, so the two middle calls run with the position
 * stashed. */

/* Wave 34 integration: takes a ProcPtr. The sole call site spends
 * `adds r0, r5, #0` immediately before the `bl` -- materialised argument setup
 * that a void(void) prototype cannot emit. That is a caller-side fact, so it
 * beats this body, which simply never reads the parameter. Unused, so the
 * definition stays byte-for-byte identical. */
void CommitSiloLaunchMove(ProcPtr proc)
{
    BackupUnitStartPosition();
    CommitUnitMove();
    RestoreUnitStartPosition();
    LockUnitSelection();
}
asm(".global sub_0804096C\n.thumb_set sub_0804096C, CommitSiloLaunchMove\n");
