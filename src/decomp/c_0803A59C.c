#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A59C.
 * sub_0803A59C @ 0x0803A59C
 */

/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Same callee, two different blobs; both results discarded, so sub_0801537C's
 * `s8` return costs nothing here. */
void EndUnitInfoPanelScripts(void)
{
    sub_0801537C(gUnknown_0849E240);
    sub_0801537C(gUnknown_0849E280);
}
asm(".global sub_0803A59C\n.thumb_set sub_0803A59C, EndUnitInfoPanelScripts\n");
