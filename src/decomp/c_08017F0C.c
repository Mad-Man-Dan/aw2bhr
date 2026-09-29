#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017F0C.
 * sub_08017F0C @ 0x08017F0C
 */

/* Clears the gUnknown_03001FF0 callback, runs EndEventScriptSlot over every
 * gUnknown_0200C528 slot EXCEPT `a`, then advances slot `a`'s own list cursor
 * by one node.
 *
 * `gUnknown_0200C528[a].unk04++` is the honest spelling of `ldr; adds #0x10;
 * str`: unk04 is a `struct Unk0200C528Node *` and the node is 0x10 bytes, so
 * the pointer increment IS the +0x10. Nothing here is byte arithmetic.
 *
 * `a` is s16 -- the sign extension is computed once for the loop's `!=` test
 * and the pre-shifted value is kept in a second register and re-`asr`ed for
 * the array index, which is agbcc's usual s16 two-use shape. The loop counter
 * is separately u8 (`lsls #0x18; lsrs #0x18` with an unsigned `bls`).
 *
 * The `return 0` is a guess in one respect only: the function has no callers
 * anywhere in the tree, so nothing constrains the width and nothing would
 * observe the value. `movs r0,#0` before the pop is what the ROM does. */
int EventOp_EndOtherScripts(s16 a)
{
    u8 i;

    gUnknown_03001FF0 = NULL;

    for (i = 0; i <= 9; i++)
    {
        if (i != a)
            EndEventScriptSlot(i);
    }

    gUnknown_0200C528[a].unk04++;
    return 0;
}
asm(".global sub_08017F0C\n.thumb_set sub_08017F0C, EventOp_EndOtherScripts\n");
