#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801EFF4.
 * sub_0801EFF4 @ 0x0801EFF4
 */

/* Pushes the whole 0x80-entry OAM shadow to OAM. `movs r1, #0xe0;
 * lsls r1, #0x13` is 0x07000000, and the count 0x80 is objects rather than
 * bytes -- CpuFastCopy's third parameter is a u16 count, the same convention
 * struct OamTransfer's objectCount already uses two blocks away. */
void CopyLoOamShadowToOam(void)
{
    CpuFastCopy(gUnknown_03002520, (void *)0x07000000, 0x80);
}
asm(".global sub_0801EFF4\n.thumb_set sub_0801EFF4, CopyLoOamShadowToOam\n");
