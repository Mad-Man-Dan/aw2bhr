#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F540.
 * sub_0803F540 @ 0x0803F540
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* PlayMusicOrSfx2 was declared `void f(s16)` until wave 11 proved it `int`; the
 * correction is invisible here because 0x1CB is a constant >255 and every
 * integer parameter type reaches the same `ldr r0,=0x1cb`. Recorded so nobody
 * reads this function as evidence for the width in either direction.
 */

void sub_0803F540(void)
{
    PlayMusicOrSfx2(0x1CB);
}
