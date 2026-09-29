#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012A74.
 * sub_08012A74 @ 0x08012A74
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Same registration shape as sub_080111AC: sub_08012A34 (already promoted in
 * src/decomp/c_08012A24.c) goes into the gUnknown_03002FA0 list. Two functions
 * in this batch call QueueVBlankCallback with two different function symbols, which
 * pins its parameter as an opaque `void *` harder than either alone -- neither
 * argument is a data object, so no data type could describe both.
 */

void sub_08012A74(void)
{
    QueueVBlankCallback((void *)sub_08012A34);
}
