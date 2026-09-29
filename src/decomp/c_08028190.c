#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028190.
 * sub_08028190 @ 0x08028190, sub_080281A0 @ 0x080281A0
 */

/* The consumer of the word sub_080281D8 parks in the slot's +0x18 -- the shared
 * displacement on a sub_080152EC result is what makes the parameter a
 * struct Unk03001470 rather than a Proc. */
void sub_08028190(struct Unk03001470 *p)
{
    if (p->unk18 != 0)
        ApplyUnitSpawnTable((void *)p->unk18);
}

/* "Is the gUnknown_08499EE4 script running?". The `movs #1` and `movs #0` are
 * split across an unconditional `b`, which is the if/else spelling rather than
 * a returned comparison -- `return sub_08015BD0(...) != -1;` goes through
 * do_store_flag and arrives with no `b` at all.
 *
 * The test is written `== -1` returning FALSE, not `!= -1` returning TRUE: the
 * ROM branches on `beq` to the `movs r0, #0` block and falls through to
 * `movs r0, #1`, and the opposite spelling inverts both. The `(s32)` cast is
 * the price of sub_08015BD0's declared `s32` first parameter. */
bool8 sub_080281A0(void)
{
    if (sub_08015BD0((s32)gUnknown_08499EE4) == -1)
        return FALSE;
    else
        return TRUE;
}
