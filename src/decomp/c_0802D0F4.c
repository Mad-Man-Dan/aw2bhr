#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D0F4.
 * sub_0802D0F4 @ 0x0802D0F4
 */

/* The same frame-entry shape as c_0802CFFC.c -- open with LockUnitSelection and
 * CloseTopMenu, close with the BackupUnitStartPosition / CommitUnitMove / RestoreUnitStartPosition
 * bracket -- with a different middle and one extra call (sub_08042998) in the
 * tail.
 *
 * The middle is the block unknown-globals.h already records for sub_08042998
 * on gPlaySt.unk0d: the zero loaded for the `unk0d == 0` test is
 * REUSED to clear gUnknown_03004074, which is why the store is `strb r5, [r4]`
 * with r5 the byte just compared. `gUnknown_03004074 = 0;` is the source --
 * agbcc's cse records the equivalence from the conditional jump, so no
 * `movs rN, #0` is emitted.
 *
 * unk32 is tested TWICE and the tests are NESTED, not sequential: the first
 * `beq` jumps past the second test to the tail, which an `if (unk32 && ...)`
 * pair could not produce. SubtractUnitFuel wants a `struct Unit *` while
 * gUnknown_030040D8 is declared `struct Unk030040D8 *`; the cast is
 * byte-neutral and the two struct names are already flagged in
 * unknown-globals.h as the same object (gUnknown_08091364 is
 * `struct Unit **` holding &gUnknown_030040D8).
 *
 * MATCHED first draft. */
void UnitMenu_Join(void)
{
    LockUnitSelection();
    CloseTopMenu();

    if (gPlaySt.savingEnabled != 0)
    {
        if (gPlaySt.fog == 0)
        {
            SubtractUnitFuel((struct Unit *)gUnknown_030040D8, gUnknown_03004074);
            gUnknown_03004074 = 0;
        }

        if (gPlaySt.savingEnabled != 0)
            SendActionCommand(0xa, gUnknown_03003F38, 0, 0);
    }

    BackupUnitStartPosition();
    sub_08042998();
    CommitUnitMove();
    RestoreUnitStartPosition();
}
asm(".global sub_0802D0F4\n.thumb_set sub_0802D0F4, UnitMenu_Join\n");
