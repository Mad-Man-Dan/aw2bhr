#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078608.
 * sub_08078608 @ 0x08078608, sub_08078658 @ 0x08078658, sub_080786A4 @ 0x080786A4
 */

int AddCoSelectGroupOrangeStar(int a)
{
    gUnknown_03005958[gUnknown_03005944] = 0;
    gUnknown_03005948[gUnknown_03005944] = 3;
    gUnknown_030058E0[a++] = 1;
    gUnknown_030058E0[a++] = 4;
    gUnknown_030058E0[a++] = 2;
    gUnknown_03005944++;

    return a;
}
asm(".global sub_08078608\n.thumb_set sub_08078608, AddCoSelectGroupOrangeStar\n");

int AddCoSelectGroupBlueMoon(int a)
{
    gUnknown_03005958[gUnknown_03005944] = 1;
    gUnknown_03005948[gUnknown_03005944] = 3;
    gUnknown_030058E0[a++] = 3;
    gUnknown_030058E0[a++] = 5;
    gUnknown_030058E0[a++] = 0x10;
    gUnknown_03005944++;

    return a;
}
asm(".global sub_08078658\n.thumb_set sub_08078658, AddCoSelectGroupBlueMoon\n");

int AddCoSelectGroupYellowComet(int a)
{
    gUnknown_03005958[gUnknown_03005944] = 3;
    gUnknown_03005948[gUnknown_03005944] = 3;
    gUnknown_030058E0[a++] = 6;
    gUnknown_030058E0[a++] = 7;
    gUnknown_030058E0[a++] = 0x12;
    gUnknown_03005944++;

    return a;
}
asm(".global sub_080786A4\n.thumb_set sub_080786A4, AddCoSelectGroupYellowComet\n");
