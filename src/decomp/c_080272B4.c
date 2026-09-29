#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080272B4.
 * sub_080272B4 @ 0x080272B4
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* The stop half of StartSupplyIconEffect's Proc_Start on tree 3. */

void EndSupplyIconEffect(void)
{
    Proc_EndEach(gUnknown_08499D2C);
}
asm(".global sub_080272B4\n.thumb_set sub_080272B4, EndSupplyIconEffect\n");
