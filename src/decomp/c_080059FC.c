#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080059FC.
 * sub_080059FC @ 0x080059FC, sub_08005AA0 @ 0x08005AA0
 */

/* Six sub_08014A5C calls at string ids 0x9EF..0x9F4 behind one sub_08012BC8
 * window open, the same shape as the promoted sub_0800518C in
 * src/decomp/c_08005154.c. 0x9F0 is `movs r3,#0x9f; lsls r3,#4` and the other
 * five come from the pool; that is the constant's own spelling, not a type
 * difference. */
void DesignRoomDrawHelpPage1(void)
{
    sub_08012BC8(gBG0TilemapBuffer, 0, 0, 0x1E, 0x14, 0);
    sub_08014A5C(3, 3, gBG0TilemapBuffer, 0x9EF, 0x8000, 0);
    sub_08014A5C(4, 5, gBG0TilemapBuffer, 0x9F0, 0x8000, 0);
    sub_08014A5C(4, 7, gBG0TilemapBuffer, 0x9F1, 0x8000, 0);
    sub_08014A5C(3, 9, gBG0TilemapBuffer, 0x9F2, 0x8000, 0);
    sub_08014A5C(4, 0xB, gBG0TilemapBuffer, 0x9F3, 0x8000, 0);
    sub_08014A5C(4, 0xD, gBG0TilemapBuffer, 0x9F4, 0x8000, 0);
    sub_08013AEC();
}
asm(".global sub_080059FC\n.thumb_set sub_080059FC, DesignRoomDrawHelpPage1\n");

/* DesignRoomDrawHelpPage1's tail without the sub_08012BC8 window open, five rows at
 * column 8 and string ids 0x9F5..0x9F9, then the promoted sub_080059E4. */
void DesignRoomDrawHelpPage2(void)
{
    sub_08014A5C(8, 4, gBG0TilemapBuffer, 0x9F5, 0x8000, 0);
    sub_08014A5C(8, 6, gBG0TilemapBuffer, 0x9F6, 0x8000, 0);
    sub_08014A5C(8, 8, gBG0TilemapBuffer, 0x9F7, 0x8000, 0);
    sub_08014A5C(8, 0xA, gBG0TilemapBuffer, 0x9F8, 0x8000, 0);
    sub_08014A5C(8, 0xC, gBG0TilemapBuffer, 0x9F9, 0x8000, 0);
    sub_08013AEC();
    sub_080059E4();
}
asm(".global sub_08005AA0\n.thumb_set sub_08005AA0, DesignRoomDrawHelpPage2\n");
