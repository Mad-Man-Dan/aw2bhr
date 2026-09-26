#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037780.
 * sub_08037780 @ 0x08037780
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Removes sub_08037790 from the 16-slot gUnknown_03000000 callback list.
 * sub_08011B5C is the remover and sub_08011B34 the inserter, both `void *`,
 * so a function argument casts -- same spelling as sub_080111AC's
 * registration. sub_08037790 is not promoted yet; `void (void)` is read off
 * its own bytes (`push {lr}` ... `pop {r0}; bx r0`, no argument register
 * read).
 */

void sub_08037780(void)
{
    sub_08011B5C((void *)sub_08037790);
}
