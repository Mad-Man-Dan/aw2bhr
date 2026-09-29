#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033404.
 * sub_08033404 @ 0x08033404, sub_0803343C @ 0x0803343C
 */

#include "proc.h"
#include "hardware.h"

/* Pumps the link session every frame and breaks its own proc once NEITHER of
 * the two 0x0861 scripts is alive -- the same shape wave 30 recorded for
 * sub_080321A8. Two `cmp r0,#0; bne` to one label is the short-circuit `&&`,
 * and neither Proc_Find result is narrowed, so both are word wide. */
void sub_08033404(ProcPtr proc)
{
    MultiBootMain(gUnknown_03003F70);

    if (Proc_Find(gUnknown_0861429C) == NULL && Proc_Find(gUnknown_08614284) == NULL)
        Proc_Break(proc);
}

/* Takes ONE argument even though nothing in the body names it: r0 is never
 * written before `bl sub_080324C4`, whose first parameter it therefore is.
 *
 * The blend register uses the `.raw` view, not the bitfield one. The five
 * target1_enable_* bits are separate 1-bit members, so setting all five would
 * be five `orr`s; the ROM has a single load / `and 0xFFE0` / `orr 0x1f` /
 * store, which is one 16-bit read-modify-write. The mask is written 0xFFE0 and
 * not ~0x1F -- the latter is the int 0xFFFFFFE0 and would put that word in the
 * pool instead. hardware.h records that both views appear in this tree and
 * that the choice is per-function. */
void sub_0803343C(int a)
{
    sub_080324C4(a, -1, 1);
    Proc_Start(gUnknown_08614284, PROC_TREE_3);
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x1F;
}
