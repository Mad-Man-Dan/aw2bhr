#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078F00.
 * sub_08078F00 @ 0x08078F00
 */

/* Steps the fade counter and unblocks the proc once it passes 15.
 *
 * The `ldrh` immediately after the `strh` on the same address is the
 * store-forwarding tell: with a plain u16 GCC forwards the value it just
 * stored and re-narrows it (`lsl #16; lsr #16; cmp`), which is one instruction
 * longer and does not match. gUnknown_03001FFC is therefore volatile -- see the
 * note on its declaration in hardware.h; all eight other users are bare scalar
 * stores of a constant and were re-checked with trymatch after the change. */

#include "hardware.h"
#include "proc.h"

void ResultsScreen_FadeToWhite_Loop(ProcPtr proc)
{
    gUnknown_03001FFC += 2;

    if (gUnknown_03001FFC > 0xf)
        Proc_Break(proc);
}
asm(".global sub_08078F00\n.thumb_set sub_08078F00, ResultsScreen_FadeToWhite_Loop\n");
