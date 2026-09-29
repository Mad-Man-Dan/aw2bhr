#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F050.
 * sub_0801F050 @ 0x0801F050, sub_0801F06C @ 0x0801F06C
 */

/* Family F070: an if/else on gUnknown_03001FE0, the "is there a deferred
 * execution context" word. F070 and F071 alternate in address order
 * (0x0801F050, 0x0801F06C, 0x0801F0AC, 0x0801F0C8, 0x0801F0E0, 0x0801F0FC) and
 * branch on the same global, so the six are one source block: three
 * frame-teardown routines, each an if/else pair followed by a bare if.
 *
 * The else-arm callee here is sub_0801E0F0, the F069 representative matched in
 * the same batch. */

void sub_0801F050(void)
{
    if (gUnknown_03001FE0)
        SyncHiOamNoCopy();
    else
        sub_0801E0F0();
}

/* Family F071: the bare-if half of the F070/F071 interleave -- see
 * sub_0801F050. `cmp r0,#0; bne` past a single `bl` is `if (!g) f();`. */

void sub_0801F06C(void)
{
    if (!gUnknown_03001FE0)
        sub_08015550();
}
