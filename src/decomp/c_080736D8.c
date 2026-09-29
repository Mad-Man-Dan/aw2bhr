#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080736D8.
 * sub_080736D8 @ 0x080736D8
 */

/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Sibling of sub_0806F2C0 and EndBgWave. */
#include "proc.h"

void EndScanlineDarkenBg0(void)
{
    Proc_EndEach(gUnknown_0861418C);
    QueueVBlankCallback((void *)ResetDma0Registers);
}
asm(".global sub_080736D8\n.thumb_set sub_080736D8, EndScanlineDarkenBg0\n");
