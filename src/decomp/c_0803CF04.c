#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CF04.
 * sub_0803CF04 @ 0x0803CF04, sub_0803CF3C @ 0x0803CF3C
 */

/* The pool word the disassembly calls `gUnknown_08091140` is not a global: the
 * ROM word there is 0x02000000, i.e. &gUnknown_02000000, and it is agbcc's own
 * -fforce-addr address-constant pool. The tell is the DOUBLE `ldr r1, [r4]` --
 * r4 holds the pool word's address and each use re-loads the object address
 * from it, exactly as documented for 0x0808E558. Naming gUnknown_02000000
 * honestly reproduces both loads and lets the build place the constant. */
void LoadSavedMapRecordIntoGMap(u8 a, int b)
{
    if (gPlaySt.savingEnabled == 0)
        ReadSaveSlot(a + 5, gUnknown_02000000);
    CopyMapRecordToGMap(b, gUnknown_02000000);
}
asm(".global sub_0803CF04\n.thumb_set sub_0803CF04, LoadSavedMapRecordIntoGMap\n");

/* Two parameters, and the second is invisible here -- r1 is forwarded to
 * LoadSavedMapRecordIntoGMap with nothing done to it. The arity is read off the CALLEE's
 * prologue, which does read r1 (`adds r5, r1, #0`).
 *
 * This one names gUnknown_02000000 ONCE and gets the ordinary one-level
 * `ldr r0, =gUnknown_02000000`, where LoadSavedMapRecordIntoGMap names it twice and gets the
 * two-level -fforce-addr form. Number of uses is the whole difference. */
void LoadSavedMapRecordWithUnits(u8 a, int b)
{
    LoadSavedMapRecordIntoGMap(a, b);
    PlaceMapRecordUnits(gUnknown_02000000);
}
asm(".global sub_0803CF3C\n.thumb_set sub_0803CF3C, LoadSavedMapRecordWithUnits\n");
