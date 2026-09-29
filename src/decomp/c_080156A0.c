#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080156A0.
 * sub_080156A0 @ 0x080156A0, sub_080156C4 @ 0x080156C4, sub_080156E8 @ 0x080156E8
 */

/* sub_080156C4's twin on sub_0801DA44 (read .unk2c) rather than sub_0801DA54
 * (read and clear it).
 *
 * The `(s16)` cast under an `int` return is real. sub_0801DA44 is promoted in
 * src/decomp/c_0801DA44.c returning `u32`, so agbcc does no re-narrowing of
 * its own after the `bl` and the `lsls #0x10; asrs #0x10` has to come from
 * this function. The note above sub_080156C4 in include/unknown-functions.h
 * used to attribute that pair to an s16-returning callee; the promoted
 * definition refutes it.
 */
int sub_080156A0(s16 a)
{
    return (s16)sub_0801DA44(gUnknown_03001470[a].unk26);
}

/* Read-and-clear of the slot's .unk2c counter -- see sub_080156A0 for why the
 * `(s16)` cast is written out rather than being agbcc re-narrowing a callee.
 */
int sub_080156C4(s16 a)
{
    return (s16)sub_0801DA54(gUnknown_03001470[a].unk26);
}

/* A narrowing forwarder to SetSlotSpriteScriptIndex. The second parameter is `void *` on
 * caller-side evidence (see include/unknown-functions.h) even though
 * SetSlotSpriteScriptIndex uses it as a table index; the cast is what produces the
 * `lsls #0x10; lsrs #0x10`.
 */
void sub_080156E8(s16 a, void *b)
{
    SetSlotSpriteScriptIndex(a, (u32)b);
}
