#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808BB0C.
 * sub_0808BB0C @ 0x0808BB0C
 */

/* fabsf, as the header's wave-13 note already predicted from sinf's
 * use of it. `a < 0.0f` picks __ltsf2 and the `blt`; the negation is __negsf2,
 * which is what `-a` compiles to with no hardware FPU. */
float sub_0808BB0C(float a)
{
    if (a < 0.0f)
        return -a;

    return a;
}
