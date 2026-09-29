#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B1EC.
 * sub_0803B1EC @ 0x0803B1EC
 */

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` behind it. Nothing about the signature is recoverable -- a leaf
 * ending in a bare `bx lr` that touches no register has no return type and no
 * argument count -- so void(void) is the weakest model, exactly as for
 * SoundMainLoopNoOp in src/decomp/c_0803B3C8.c. */

void EmptySlotHook(void)
{
}
asm(".global sub_0803B1EC\n.thumb_set sub_0803B1EC, EmptySlotHook\n");
