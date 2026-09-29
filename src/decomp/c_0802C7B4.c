#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C7B4.
 * sub_0802C7B4 @ 0x0802C7B4, sub_0802C7DC @ 0x0802C7DC
 */

/* Install the proc-list script named by the current gUnknown_085C77A0 record.
 *
 * unk08 is a POINTER, and this is the discriminating use that settles it: the
 * word goes straight into StartEventScript, whose declared parameter is
 * `const u8 *`. sub_0802C7FC only null-tests the same word, which cannot tell a
 * u32 from a pointer -- the "a byte-neutral wrong type has no oracle" case in
 * docs/agbcc-codegen.md, resolved here by a second, differently-shaped reader.
 *
 * The `adds r1, #8` onto the base rather than an `ldr` displacement is the
 * word-member hoist an array global's `g[i].member` always produces, already
 * recorded on the struct; it is not evidence of a different aggregate shape.
 */

void sub_0802C7B4(void)
{
    CloseTopMenu();
    StartEventScript(gUnknown_085C77A0[gPlaySt.mapID].unk08);
}

/* The gUnknown_030044A0 counterpart of sub_0802CD54's gUnknown_030040F0 stash:
 * same u8 second argument, same word `str` keeping PROMOTE_MODE's shift pair
 * live, and both globals are already declared u32.
 *
 * The tail sets gUnknown_03003334 to 2 with `strh`, agreeing with the u16
 * declaration src/decomp/c_0802C480.c's `= 0` established.
 */

void sub_0802C7DC(int a1, u8 a2)
{
    gUnknown_030044A0 = a2;
    CloseTopMenu();
    gUnknown_03003334 = 2;
}
