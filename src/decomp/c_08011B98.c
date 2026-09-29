#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011B98.
 * sub_08011B98 @ 0x08011B98
 */

/* Runs every non-null entry of the 16-slot callback list B
 * (gUnknown_03000000); ClearVBlankCallbackQueue's twin sub_08011B18 is the reset half.
 *
 * `bl _call_via_r0` is ordinary agbcc output for an indirect THUMB call, and
 * the r0 index says the callee takes no arguments.
 *
 * The ROM LOADS THE SLOT TWICE -- once for the null test, once for the call --
 * while CSE-ing the address into r1 across the branch. That is a volatile
 * tell, and it is why the pointer is re-qualified through a local rather than
 * spelled `gUnknown_03000000[i]` directly: the plain spelling lets gcse fold
 * the second load away and comes out two bytes short. The qualifier is applied
 * LOCALLY on purpose -- gUnknown_03000000 stays `void *[]` in
 * include/unknown-globals.h, since widening a shared global to suit one
 * function is exactly what the wave briefs forbid, and the other readers of
 * this list are not matched yet to corroborate it.
 *
 * Casting at each use instead of binding the local does NOT work: it forces
 * the base address into a `.rodata` pool word and adds an indirection. */
void RunVBlankHooks(void)
{
    u16 i;

    for (i = 0; i <= 0xf; i++)
    {
        void (*volatile *fns)(void);

        fns = (void (*volatile *)(void))gUnknown_03000000;

        if (fns[i] != NULL)
            fns[i]();
    }
}
asm(".global sub_08011B98\n.thumb_set sub_08011B98, RunVBlankHooks\n");
