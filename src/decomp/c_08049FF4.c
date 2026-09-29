#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049FF4.
 * sub_08049FF4 @ 0x08049FF4
 */

/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Same shape as EndUnitInfoPanelScripts over the neighbouring pair of blobs. */
void LanguageSelect_EndScripts(void)
{
    sub_0801537C(gUnknown_084C3814);
    sub_0801537C(gUnknown_084C3824);
}
asm(".global sub_08049FF4\n.thumb_set sub_08049FF4, LanguageSelect_EndScripts\n");
