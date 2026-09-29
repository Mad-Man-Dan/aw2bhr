#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072A3C.
 * sub_08072A3C @ 0x08072A3C
 */

/* src/decomp/c_08013D00.c's twin on the same four pointer globals, differing in
 * three readable ways.
 *
 * (1) The index is the SUBSCRIPT form, `&(*pp)[y * 32 + x]`: the ROM adds first
 * and scales once (`lsls #5; adds; lsls #1`), where sub_08013D00's
 * pointer-addition form scales each term separately (`lsls #6` then `lsls #1`).
 * The deref stays inside the returned expression so the multiply is issued
 * before the `ldr`, which is what forces x out of r1 with `adds r3, r1, #0`.
 *
 * (2) The default arm returns NULL instead of sharing `case 0:`. The case set
 * is still {0, 1, 2, 3}, which is what makes agbcc pivot its decision tree on 1
 * (`cmp #1; beq` then `cmp #1; bgt`) rather than on 2 -- three cases alone
 * bisect at 2.
 *
 * (3) THE `goto` IS LOAD-BEARING, and this is the only spelling of five probed
 * that lands. In the ROM the `movs r0, #0` block sits AFTER the shared tail,
 * and case 3 falls THROUGH into that tail with no branch. A `default: return
 * NULL;` inside the switch is a case body, so agbcc emits it with the other
 * four -- before the tail -- and case 3 then needs a `b` to reach the tail:
 * same size, 22 of 76 bytes different. Jumping out of the switch to a label
 * past the tail's `return` is what puts the failure block last. */
u16 *GetTmOffsetById(int which, int x, int y)
{
    u16 **pp;

    switch (which)
    {
    case 0:
        pp = &gBG0TilemapBuffer;
        break;
    case 1:
        pp = &gBG1TilemapBuffer;
        break;
    case 2:
        pp = &gBG2TilemapBuffer;
        break;
    case 3:
        pp = &gBG3TilemapBuffer;
        break;
    default:
        goto fail;
    }

    return &(*pp)[y * 32 + x];

fail:
    return NULL;
}
asm(".global sub_08072A3C\n.thumb_set sub_08072A3C, GetTmOffsetById\n");
