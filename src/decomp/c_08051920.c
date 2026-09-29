#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051920.
 * sub_08051920 @ 0x08051920
 */

/* RaiseHitDoneStageFlag's counterpart on the OTHER side (`a ^ 1`) and with the screen
 * test inverted: it raises the same redraw bit when the opposite side's entry
 * is idle and this side is NOT the one on screen, then stops the proc the
 * caller names.
 *
 * `c` is `s16`: the entry narrowing is PROMOTE_MODE's zero-extend and the
 * `lsls #0x10; asrs #0x10` at the use is the sign, CSEd across all three uses
 * (the `!= -1` guard and both calls, whose own parameters are `s16` and cost
 * nothing on top). IsSlotScriptActiveAt returns `bool8`, hence the bare
 * `lsls #0x18; cmp #0` truth test with no `lsrs`. */
void EndMissileHitEffect(u16 a, u16 b, s16 c)
{
    if (gUnknown_02029A10[a ^ 1].entries[b].unk1a == 0 && a != gUnknown_0300450C)
        gUnknown_02029664 |= 0x40;

    if (c != -1 && IsSlotScriptActiveAt(c))
        EndSlotScriptAt(c);
}
asm(".global sub_08051920\n.thumb_set sub_08051920, EndMissileHitEffect\n");
