#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049EB4.
 * sub_08049EB4 @ 0x08049EB4
 */

/* Five stops in a row, and the last four name the SAME blob -- the ROM keeps
 * gUnknown_084C3244 in r4 across them, which is what pays for the `push {r4}`.
 * A single call plus a loop would have emitted a counter; this is four separate
 * statements. */
void DefeatFlow_OnEnd(void)
{
    sub_0801537C(gUnknown_084C325C);
    sub_0801537C(gUnknown_084C3244);
    sub_0801537C(gUnknown_084C3244);
    sub_0801537C(gUnknown_084C3244);
    sub_0801537C(gUnknown_084C3244);
}
asm(".global sub_08049EB4\n.thumb_set sub_08049EB4, DefeatFlow_OnEnd\n");
