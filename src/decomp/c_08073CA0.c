#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073CA0.
 * sub_08073CA0 @ 0x08073CA0
 */

/* Existence predicate for gUnknown_086141DC; its starter is the immediately
 * preceding StartPolygonWipe. Identical to IsCircleWipeActive but for the script. */

#include "proc.h"

int IsPolygonWipeActive(void)
{
    return Proc_Find(gUnknown_086141DC) != 0;
}
asm(".global sub_08073CA0\n.thumb_set sub_08073CA0, IsPolygonWipeActive\n");
