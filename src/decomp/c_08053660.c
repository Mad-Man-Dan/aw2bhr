#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053660.
 * sub_08053660 @ 0x08053660
 */

/* F086 -- `push {lr}; lsls #0x10; lsrs #0x10; bl` and nothing else.
 *
 * The narrowing is PROMOTE_MODE on a declared-narrow parameter: agbcc
 * zero-extends every sub-word parameter into its pseudo at entry. `int` with
 * an explicit `(u16)` cast at the call is byte-identical here and a probe
 * cannot separate the two, so the choice is made callee-side --
 * sub_08053670 takes the u16 unit index the rest of that family takes,
 * and a wrapper that exists only to forward it takes the same thing.
 */
void sub_08053660(u16 a)
{
    sub_08053670(a);
}
