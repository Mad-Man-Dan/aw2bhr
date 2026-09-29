#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E0F0.
 * sub_0801E0F0 @ 0x0801E0F0
 */

/* Family F069: hide a run of OAM entries, then reset the matching counter.
 * HideOamObjects(a, n) is already promoted as `void (int, int)` and blanks `n`
 * objects starting at object `a`, so the two literals are an OAM range and the
 * `strh 0` is the shadow counter for that range going back to empty.
 *
 * The near-twin ClearOamShadowResetCursors at 0x0801EF6C is the same call followed by BOTH
 * stores (gUnknown_03002B54 = 0x10 and gUnknown_03001FE4 = 0), which is why it
 * is not in F069 -- and it is what shows the two globals are independent
 * counters rather than one being a mistake for the other. */

void ClearOamShadow(void)
{
    HideOamObjects(0, 0x80);
    gUnknown_03002B54 = 0;
}
asm(".global sub_0801E0F0\n.thumb_set sub_0801E0F0, ClearOamShadow\n");
