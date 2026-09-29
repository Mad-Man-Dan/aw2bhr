#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011218.
 * sub_08011218 @ 0x08011218
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* The stop half of a start/stop pair: StartFadeScreenLines Proc_Start's
 * ProcScr_FadeScreenLines and this ends every instance of it.
 */

void EndFadeScreenLines(void)
{
    Proc_EndEach(ProcScr_FadeScreenLines);
}
asm(".global sub_08011218\n.thumb_set sub_08011218, EndFadeScreenLines\n");
