#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073918.
 * sub_08073918 @ 0x08073918
 */

/* The existence predicate for gUnknown_086141B4, whose starter is the
 * neighbouring StartCircleWipe. Same shape as the 23 wrappers described on the
 * "start/exists proc-script family" comment in unknown-globals.h, except that
 * this script's starter takes a payload rather than being the 20-byte
 * PROC_TREE_3 form.
 *
 * `return X != 0;` and not `if (X) return 1; return 0;`: the boolean form keeps
 * the true-value block as a fallthrough and never materialises the zero, which
 * is the 4-byte signal the control-flow table in docs/agbcc-codegen.md gives. */

#include "proc.h"

int IsCircleWipeActive(void)
{
    return Proc_Find(gUnknown_086141B4) != 0;
}
asm(".global sub_08073918\n.thumb_set sub_08073918, IsCircleWipeActive\n");
