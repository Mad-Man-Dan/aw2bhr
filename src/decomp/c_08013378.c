#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013378.
 * sub_08013378 @ 0x08013378
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* The stop half of StartScreenShake's start (Proc_StartBlocking or Proc_Start on
 * tree 3, depending on its ProcPtr argument).
 */

void EndScreenShake(void)
{
    Proc_EndEach(gUnknown_084893AC);
}
asm(".global sub_08013378\n.thumb_set sub_08013378, EndScreenShake\n");
