#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080513FC.
 * sub_080513FC @ 0x080513FC
 */

/* Raises bit 6 of gUnknown_02029664 -- a "redraw needed" flag -- when a proc
 * has just finished, the entry it belonged to is idle, and the side is the one
 * currently on screen.
 *
 * sub_080156C4 is declared `int (s16)` and its result is re-narrowed
 * `lsls #0x10; lsrs #0x10` before the `cmp #1`, which is a `(u16)` cast at this
 * use and not evidence about its return type -- the note on that function in
 * include/unknown-functions.h already settles the type from its own body. */
void RaiseHitDoneStageFlag(u16 a, u16 b, s16 c)
{
    if ((u16)sub_080156C4(c) == 1
        && gUnknown_02029A10[a].entries[b].unk1a == 0
        && a == gUnknown_0300450C)
        gUnknown_02029664 |= 0x40;
}
asm(".global sub_080513FC\n.thumb_set sub_080513FC, RaiseHitDoneStageFlag\n");
