#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070EF4.
 * sub_08070EF4 @ 0x08070EF4
 */

/* CgbOscOff. A `switch`, not an if-chain: the ROM's `cmp #2; beq / cmp #2; bgt`
 * binary-search dispatch is what agbcc emits for a switch over 1..3 with a
 * default, and the three tails are merged by cross-jumping -- case 1 falls into
 * the shared `movs r0,#0x80; strb` after `adds r1,#2`, cases 2 and default share
 * `movs r0,#8; strb; adds r1,#4`, and case 3 joins at the final `strb` alone.
 * That merging is why each arm materialises only its own register base. */
void CgbOscOff(u8 chan)
{
    switch (chan)
    {
    case 1:
        *(volatile u8 *)0x04000063 = 8;
        *(volatile u8 *)0x04000065 = 0x80;
        break;
    case 2:
        *(volatile u8 *)0x04000069 = 8;
        *(volatile u8 *)0x0400006D = 0x80;
        break;
    case 3:
        *(volatile u8 *)0x04000070 = 0;
        break;
    default:
        *(volatile u8 *)0x04000079 = 8;
        *(volatile u8 *)0x0400007D = 0x80;
        break;
    }
}
asm(".global sub_08070EF4\n.thumb_set sub_08070EF4, CgbOscOff\n");
