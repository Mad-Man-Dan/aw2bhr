#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007328.
 * sub_08007328 @ 0x08007328
 */

/* A modular decrement over a ring whose size is 10 or 8 depending on the s8
 * flag at +7.  `n - 1` is bound to a local rather than folded into the sum:
 * the ROM computes it into its own register (`subs r1, r3, #1`) BEFORE the
 * `ldrsh`, whereas `unk3a + n - 1` associates as `(unk3a + n) - 1` and emits
 * the subtraction after the load.
 *
 * `pop {r1}` says the function returns a value, and r0 already holds the
 * stored halfword, so the return costs nothing. */
int DesignRoomGetPreviousRingIndex(void)
{
    struct ActiveMap *p = gActiveMap;
    int n;
    int m;
    int v;

    n = (p->editMode == 0) ? 10 : 8;
    m = n - 1;
    v = p->ringIndex + m;
    if (v >= n)
        v -= n;
    p->previousRingIndex = v;
    return v;
}
asm(".global sub_08007328\n.thumb_set sub_08007328, DesignRoomGetPreviousRingIndex\n");
