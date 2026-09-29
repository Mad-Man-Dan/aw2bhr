#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037F58.
 * sub_08037F58 @ 0x08037F58
 */

/* Family F011: `push {lr}; bl f; ldr r0,=g; movs r1,#N; bl h; pop {r0}; bx r0`.
 * Two statements and NOT a nest -- r0 is overwritten by the pool `ldr` between
 * the two calls, so nothing can survive from the first one. `pop {r0}` is the
 * void epilogue.
 *
 * sub_08037F18 is a bare `bx lr` -- an empty function, not a missing one.
 *
 * The `(s32)` cast is forced by sub_080152C0's declaration, which
 * unknown-functions.h flags as SUSPECT (the parameter is almost certainly
 * `const void *`). It is byte-neutral -- `ldr r0,=gSym` either way -- so this
 * is one more call site paying the cast, and it comes out free. */
void StartBg3AutoScroll(void)
{
    sub_08037F18();
    sub_080152C0((s32)gUnknown_0849D55C, 0);
}
asm(".global sub_08037F58\n.thumb_set sub_08037F58, StartBg3AutoScroll\n");
