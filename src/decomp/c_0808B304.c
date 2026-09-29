#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B304.
 * sub_0808B304 @ 0x0808B304
 */

/* Runs the chip-specific routine in r1 and turns a non-zero result into the
 * error code 0x8004. `bl _call_via_r1` is an ordinary one-argument indirect
 * call -- the register index counts the arguments -- so the second parameter is
 * the function pointer and the first is what it receives.
 *
 * The test is `!= 0` with the arms in this order, for the same layout reason as
 * IsSeaAt: the ROM's `bne` reaches the 0x8004 pool load and `movs #0`
 * falls through, so the then-arm is the error return. */
int sub_0808B304(int a, int (*f)(int))
{
    if (f(a) != 0)
        return 0x8004;
    else
        return 0;
}
