#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014DA8.
 * sub_08014DA8 @ 0x08014DA8
 */

/* Installs the arena and reports failure. Both arguments are forwarded
 * untouched to HeapInitBuffer(void *, u32), which is the only place the arity
 * shows -- neither register is written before the `bl`.
 *
 * THE ADDRESS OF gUnknown_03000050 IS BOUND TO A LOCAL. The ROM materialises
 * that pool word into r4 BEFORE the call -- which is what costs the function
 * its `push {r4, lr}` -- and writing the plain `gUnknown_03000050 = f(...)`
 * puts the `ldr` after the `bl` in r0 instead, with no r4 saved at all
 * (measured with compile_probe). This is the same idiom already recorded on
 * src/decomp/c_08014668.c for gUnknown_0200C020.
 *
 * The declaration in unknown-functions.h deliberately has an EMPTY parameter
 * list, so this prototyped definition is what now fixes the types; `void *`
 * and `u32` are unchanged by the default argument promotions, so AgbMain's
 * existing call stays compatible. */
int HeapInit(void *buf, u32 size)
{
    int *p = &gUnknown_03000050;

    *p = HeapInitBuffer(buf, size);

    if (*p == -1)
        return *p;

    return 0;
}
asm(".global sub_08014DA8\n.thumb_set sub_08014DA8, HeapInit\n");
