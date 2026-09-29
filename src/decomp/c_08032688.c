#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032688.
 * sub_08032688 @ 0x08032688
 */

/* F079 -- a three-argument forwarder that passes its own argument through
 * untouched and supplies the other two as literals.
 *
 * The forwarded argument costs no instruction, so its width and its arity are
 * invisible here; both come off LinkScreenInit's prologue, which is
 * `mov r8,r0; adds r6,r1,#0; lsls r2,#0x18; lsrs r7,r2,#0x18`. The `movs r1; movs r2` order is
 * agbcc's argument setup grouped by operand class -- every `mov #imm8`
 * together -- not source order.
 */
void sub_08032688(int a)
{
    LinkScreenInit(a, 8, 0);
}
