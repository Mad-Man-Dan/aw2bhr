#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F2C0.
 * sub_0806F2C0 @ 0x0806F2C0
 */

/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Two different callees. The second pool word is a FUNCTION address, so it
 * needs the `(void *)` cast QueueVBlankCallback's declared parameter forces.
 * NOT asm-resident despite the address: 0x0806F2C0 sits below the m4a span's
 * 0x0806F734 lower bound, it is absent from data/asm-resident.json, and it
 * takes nothing in r1 -- the driver's hand-written half is the r1-convention
 * one. */
#include "proc.h"

void sub_0806F2C0(void)
{
    Proc_EndEach(gUnknown_08614200);
    QueueVBlankCallback((void *)ResetDma0Registers);
}
