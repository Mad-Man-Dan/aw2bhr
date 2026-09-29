#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072A18.
 * sub_08072A18 @ 0x08072A18
 */

/* A halfword "add a bias into a copy" loop: the same shape as AddToHalfwords two
 * files down, except the destination is a second pointer rather than the source
 * itself. `size` counts BYTES (`subs r2, #2` per halfword) and is signed --
 * `ble`/`bgt`, and the guard is duplicated at the top, so the decrement is the
 * last statement of the body rather than part of the condition.
 * `delta` is u16: the `lsls #16; lsrs #16` ahead of the loop is the fill-loop
 * parameter narrowing, and `adds r0, r3, r0` puts the value's own register
 * first, which is the narrow-parameter operand order AddToHalfwords records. */

void PutTmLinear(u16 * src, u16 * dst, int size, u16 delta)
{
    while (size > 0)
    {
        *dst = *src + delta;
        src++;
        dst++;
        size -= 2;
    }
}
asm(".global sub_08072A18\n.thumb_set sub_08072A18, PutTmLinear\n");
