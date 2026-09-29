#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039914.
 * sub_08039914 @ 0x08039914
 */

#include "proc.h"

/* Family F032, exemplar WaitForFade in src/decomp/c_08071F88.c.
 *
 * The predicate takes NO argument. `adds r4, r0, #0` followed by a `bl` with
 * r0 untouched looks like `pred(proc)`, but arity is invisible in a
 * pass-through and the answer is on the callee side: sub_08019850 is already
 * promoted as `bool8 sub_08019850(void)` (src/decomp/c_08019850.c). The
 * incoming r0 is simply dead across the call; r4 exists only to survive it.
 *
 * `lsls r0, r0, #0x18` before the `cmp` is sub_08019850's bool8 return being
 * narrowed, exactly as in the exemplar -- an int-returning predicate emits a
 * bare `cmp r0, #0`. */

void sub_08039914(ProcPtr proc)
{
    if (sub_08019850() == 0)
        Proc_Break(proc);
}
