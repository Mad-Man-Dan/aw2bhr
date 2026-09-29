#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B33C.
 * sub_0803B33C @ 0x0803B33C
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Stop half of StartSpinningSlotSprite, a long builder that also names this symbol.
 * Seven functions in this batch call sub_0801537C with seven different ROM
 * blobs; between them they pin the parameter as an opaque pointer to script
 * data and not to any one object's type.
 */

void sub_0803B33C(void)
{
    sub_0801537C(gUnknown_0849E700);
}
