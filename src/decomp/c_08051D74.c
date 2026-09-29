#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051D74.
 * StartFigureFall @ 0x08051D74
 */

/* MATCHED, first draft. The slot teardown the DeathHandler_Tank / DeathHandler_Air
 * install family calls: clear the entry's unk00, raise the per-slot flag in
 * gUnknown_02029B80, and if the entry still holds a live proc id, retire it.
 *
 * The entry address is written out in full at each of the four uses rather than
 * bound to a `struct Unk02029A10 *` local. That is measured, not style: a
 * pointer local reverses the order of the two offset terms, emitting `a * 0xb4`
 * before `b * 0x24` where the ROM has the pool word first, then `b * 0x24`,
 * then `a * 0xb4`. The matched DeathHandler_Tank has the same shape from the same
 * inline spelling.
 *
 * IsSlotScriptActiveAt returns `bool8` -- copied from its promoted definition, not
 * re-derived -- which is what makes the test a bare `lsls #0x18; cmp #0` with
 * no `lsrs`.
 *
 * sub_080156E8's second parameter is declared `void *` because this call site
 * passes the gUnknown_02029BA8 word with no narrowing; the callee narrows it to
 * u16 itself and its matched tail-callee uses it as an index. That
 * disagreement is real and is written up in include/unknown-functions.h. */
void StartFigureFall(u16 a, u16 b)
{
    gUnknown_02029A10[a].entries[b].unk00 = 0;
    gUnknown_02029B80[a][b] = 1;

    if (gUnknown_02029A10[a].entries[b].unk18 != -1
        && IsSlotScriptActiveAt(gUnknown_02029A10[a].entries[b].unk18))
        sub_080156E8(gUnknown_02029A10[a].entries[b].unk18,
            gUnknown_02029BA8[a].unk14);
}

asm(".global sub_08051D74\n.thumb_set sub_08051D74, StartFigureFall\n");
