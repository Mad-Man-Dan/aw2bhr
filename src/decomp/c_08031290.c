#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031290.
 * sub_08031290 @ 0x08031290
 */

/* A five-argument forwarder with every argument a literal. The fifth goes on
 * the stack, which is why the `movs r0,#0; str r0,[sp]` comes FIRST -- agbcc
 * emits stack arguments before register ones, then groups the register loads
 * by operand class. */

void LinkStartPrompt_Loop(void)
{
    DrawOamObject(0x4f, 0x8c, 0x70, 0, 0);
}
asm(".global sub_08031290\n.thumb_set sub_08031290, LinkStartPrompt_Loop\n");
