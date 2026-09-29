#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801EFC0.
 * CopyOamShadowToOam2 @ 0x0801EFC0
 */

/* MATCHED. Byte-for-byte the same function as CopyOamShadowToOam -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void CopyOamShadowToOam2(void)
{
    CpuFastCopy(gUnknown_03002520, (void *)0x07000000, 0x400);
}

asm(".global sub_0801EFC0\n.thumb_set sub_0801EFC0, CopyOamShadowToOam2\n");
