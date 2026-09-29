#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801AFF4.
 * sub_0801AFF4 @ 0x0801AFF4
 */

/* Latches "a flash chip was identified" into gUnknown_0200CD0C: the raw
 * IdentifyFlash result first, then its logical negation.
 *
 * Two things here are spelling-sensitive and both were probed.
 *
 * The `movs #1 / b / movs #0` split across an unconditional branch is a real
 * `if / else` with an assignment in each arm, cross-jumped to the single
 * `strb` at the join. Written as `g = (g == 0)` -- or with a `? 1 : 0` -- agbcc
 * presets 0 and conditionally overwrites, with no unconditional branch.
 *
 * The global is reached through a NAMED POINTER. Written as
 * `gUnknown_0200CD0C = ...` the body is otherwise identical, but the address is
 * materialised AFTER the `bl` into a caller-saved register; the ROM computes it
 * first and keeps it in r4 across the call, which costs the r4 push/pop pair
 * the two spellings differ by. */
void DetectFlash(void)
{
    u8 *p = &gUnknown_0200CD0C;

    *p = IdentifyFlash();

    if (*p == 0)
        *p = 1;
    else
        *p = 0;
}
asm(".global sub_0801AFF4\n.thumb_set sub_0801AFF4, DetectFlash\n");
