#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019290.
 * sub_08019290 @ 0x08019290
 */

/* Find the slot a script is installed in, or -1. The `s16 i` loop shape of
 * src/decomp/c_08019260.c, with the early `return i` taking the value straight
 * out of the sign-extended counter (`adds r0, r1, #0`) and needing no return
 * narrowing. The literal pool lands in the middle of the function, between the
 * `b` of the early return and the increment block -- that placement falls out
 * of the block order and is not a spelling choice. */
s16 FindEventScriptSlot(const u8 *a)
{
    s16 i;

    for (i = 0; i < 10; i++)
    {
        if (gUnknown_0200C528[i].unk00 == (struct Unk0200C528Node *)a)
            return i;
    }
    return -1;
}
asm(".global sub_08019290\n.thumb_set sub_08019290, FindEventScriptSlot\n");
