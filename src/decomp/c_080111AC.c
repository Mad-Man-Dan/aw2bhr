#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080111AC.
 * sub_080111AC @ 0x080111AC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Registers sub_080111BC (the DMA0-disarm leaf, src/decomp/c_080111BC.c) into
 * the 16-slot gUnknown_03002FA0 list. The `(void *)` cast is what
 * QueueVBlankCallback's `void *` parameter forces and is exactly how the two matched
 * siblings sub_0802C260/sub_0802C270 spell the same registration
 * (src/decomp/c_0802C260.c). The pool word is a relocation against a FUNCTION
 * symbol, which per docs/agbcc-codegen.md is the only thing that distinguishes
 * it from a >255 integer constant -- the instruction sequence is identical
 * either way, so naming the symbol is load-bearing and the cast is free.
 */

void FadeScreenLines_CB_080111AD(void)
{
    QueueVBlankCallback((void *)sub_080111BC);
}

asm(".global sub_080111AC\n.thumb_set sub_080111AC, FadeScreenLines_CB_080111AD\n");
