#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037F70.
 * sub_08037F70 @ 0x08037F70
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Stop half of StartBg3AutoScroll's `sub_080152C0(gUnknown_0849D55C, 0)`. Note
 * gUnknown_0849D55C sits immediately before gUnknown_0849D56C, which IS a proc
 * script -- the two kinds are interleaved in ROM, so address adjacency is not
 * evidence of kind. The consumer is.
 */

void EndBg3AutoScroll(void)
{
    sub_0801537C(gUnknown_0849D55C);
}
asm(".global sub_08037F70\n.thumb_set sub_08037F70, EndBg3AutoScroll\n");
