#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080735D0.
 * sub_080735D0 @ 0x080735D0
 */

/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Sibling of sub_0806F2C0 and EndScanlineDarkenBg0: three wrappers that end one proc
 * script each and then re-register the same ResetDma0Registers DMA0 shutdown. */
#include "proc.h"

void EndBgWave(void)
{
    Proc_EndEach(gUnknown_08614134);
    QueueVBlankCallback((void *)ResetDma0Registers);
}
asm(".global sub_080735D0\n.thumb_set sub_080735D0, EndBgWave\n");
