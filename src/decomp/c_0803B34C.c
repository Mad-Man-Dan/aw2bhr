#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B34C.
 * sub_0803B34C @ 0x0803B34C
 */

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` behind it. It sits immediately in front of the sound-setter
 * run at 0x0803B350, so it is almost certainly the wrapper for an entry point
 * this build compiled away rather than dead code. Nothing about the signature
 * is recoverable; void(void) is the weakest model. See SoundMainLoopNoOp in
 * src/decomp/c_0803B3C8.c. */

void sub_0803B34C(void)
{
}
