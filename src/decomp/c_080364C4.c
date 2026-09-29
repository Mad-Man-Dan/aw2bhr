#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080364C4.
 * sub_080364C4 @ 0x080364C4
 */

/* `pop {r1}` is the value-returning epilogue, and the declared `void *` return
 * is HeapMalloc's own -- this is a pass-through allocation of 0xa20 bytes.
 *
 * The constant is a PLAIN LITERAL despite `movs #0xa2; lsls #4` using a
 * non-minimal shift (0xa20 has five trailing zeros, so 0x51 << 5 would also
 * build it). The wave-23 "non-minimal shift means a named constant local" rule
 * does not apply to agbcc's own constant synthesiser here: writing 0xa20
 * directly reproduces the shift of 4 exactly. */
void *AllocMoveSlideGfxBuffer(void)
{
    return HeapMalloc(0xa20);
}
asm(".global sub_080364C4\n.thumb_set sub_080364C4, AllocMoveSlideGfxBuffer\n");
