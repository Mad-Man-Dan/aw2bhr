#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080192C4.
 * sub_080192C4 @ 0x080192C4, sub_080192EC @ 0x080192EC
 */

/* The parameter is the slot itself and not an index: sub_080192EC is
 * `sub_080192C4(&gUnknown_0200C528[i])`.
 *
 * The `p->unk00 = NULL` really is written in BOTH arms, and that is what the
 * ROM says rather than a guess. The store sits after the branch on both paths,
 * with `movs r0, #0` in front of it only on the fall-through; on the taken path
 * r0 already holds the zero the `cmp` just tested, so gcc reuses it and the arm
 * shrinks to a bare `str r0, [r1]`. Hoisting the store above the test instead
 * (the `tmp = p->unk00; p->unk00 = NULL; if (tmp)` spelling) puts the `movs`
 * and `str` before the `cmp` and does not match. */
void sub_080192C4(struct Unk0200C528 *p)
{
    if (p->unk00 != NULL)
    {
        p->unk00 = NULL;
        if (!sub_08019260())
        {
            sub_08017E80();
            StartQueuedEventScript();
        }
    }
    else
    {
        p->unk00 = NULL;
    }
}

void sub_080192EC(s16 a)
{
    sub_080192C4(&gUnknown_0200C528[a]);
}
