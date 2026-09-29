#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016E04.
 * sub_08016E04 @ 0x08016E04, sub_08016E14 @ 0x08016E14, sub_08016E3C @ 0x08016E3C
 */

/* `if (C) return A; return B;` in that order, not a ternary and not
 * `return a != 0;` -- a returned comparison goes through do_store_flag and
 * arrives with no unconditional branch at all. Which way round the two
 * constants sit is the whole content here: writing it as
 * `if (a) return TRUE; else return FALSE;` inverts the branch and lays the 0
 * out first. The bare `lsls #0x10` with no `lsrs` is the truth test of a
 * 16-bit parameter. */
bool8 sub_08016E04(u16 a)
{
    if (a == 0)
        return FALSE;
    return TRUE;
}

/* The save half: PackProfileRecord copies the live blocks into the buffer and
 * returns the byte count 0x5CC, which becomes sub_0801A7D8's third argument.
 * The nesting is real -- sub_0801A7D8 reads r2 -- and the argument setup order
 * (`adds r2,r0,#0`, then `movs r0,#0`, then `adds r1,r4,#0`) is the ordinary
 * grouping by operand class. */
void WriteProfile(void)
{
    MarkProfileSaved();
    sub_0801A7D8(0, gUnknown_02000000, PackProfileRecord(gUnknown_02000000));
    sub_0803D48C();
}
asm(".global sub_08016E14\n.thumb_set sub_08016E14, WriteProfile\n");

/* The load half of WriteProfile: ReadSaveSlot reads the buffer back and returns
 * non-zero on failure, in which case the unpack is skipped. That branch is what
 * proves ReadSaveSlot is `int` and not the `void` it was declared as. */
void LoadProfile(void)
{
    if (ReadSaveSlot(0, gUnknown_02000000) == 0)
        UnpackProfileRecord(gUnknown_02000000);
}
asm(".global sub_08016E3C\n.thumb_set sub_08016E3C, LoadProfile\n");
