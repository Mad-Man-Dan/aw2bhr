#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080786F0.
 * sub_080786F0 @ 0x080786F0
 */

int AddCoSelectGroupGreenEarth(int a)
{
    gUnknown_03005958[gUnknown_03005944] = 2;
    gUnknown_03005948[gUnknown_03005944] = 3;
    gUnknown_030058E0[a++] = 8;
    gUnknown_030058E0[a++] = 9;
    gUnknown_030058E0[a++] = 0x11;
    gUnknown_03005944++;

    return a;
}
asm(".global sub_080786F0\n.thumb_set sub_080786F0, AddCoSelectGroupGreenEarth\n");
