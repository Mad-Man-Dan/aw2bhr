#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AA64.
 * sub_0806AA64 @ 0x0806AA64
 */

/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Same callee, two different proc scripts. */
#include "proc.h"

void EndMeteorImpactSubProcs(void)
{
    Proc_EndEach(gUnknown_085815D0);
    Proc_EndEach(gUnknown_085815E8);
}
asm(".global sub_0806AA64\n.thumb_set sub_0806AA64, EndMeteorImpactSubProcs\n");
