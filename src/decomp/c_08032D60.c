#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032D60.
 * sub_08032D60 @ 0x08032D60
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* The stop half of StartLinkMapPick's Proc_Start. */

void EndLinkMapPick(void)
{
    Proc_EndEach(gUnknown_0849B6B0);
}
asm(".global sub_08032D60\n.thumb_set sub_08032D60, EndLinkMapPick\n");
