#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E108.
 * sub_0803E108 @ 0x0803E108, sub_0803E158 @ 0x0803E158, sub_0803E1B0 @ 0x0803E1B0, sub_0803E208 @ 0x0803E208, sub_0803E260 @ 0x0803E260, sub_0803E2B8 @ 0x0803E2B8
 */

void sub_0803E108(int a1, int a2, int a3, int a4)
{
    struct Unk02028360Pos pos;

    sub_0803DF98(8, &pos);
    sub_0803E088(a1 - pos.unk00, a2 - pos.unk02, a3, a4, 8, 0, 0, 0, 0, 0);
}

void AddLaserInventionRecord(int a1, int a2, int a3, int a4)
{
    struct Unk02028360Pos pos;

    sub_0803DF98(1, &pos);
    sub_0803E088(a1 - pos.unk00, a2 - pos.unk02, 1, 1, 1, 0x63, a3, a4, 0, 0x32);
}
asm(".global sub_0803E158\n.thumb_set sub_0803E158, AddLaserInventionRecord\n");

void sub_0803E1B0(int a1, int a2, int a3, int a4, int a5, int a6)
{
    struct Unk02028360Pos pos;

    sub_0803DF98(2, &pos);
    sub_0803E088(a1 - pos.unk00, a2 - pos.unk02, a3, a4, 2, 0, a5, a6, 0, 0x32);
}

void AddCannonInventionRecord(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    struct Unk02028360Pos pos;

    sub_0803DF98(3, &pos);
    sub_0803E088(a1 - pos.unk00, a2 - pos.unk02, a3, a4, 3, 0x63, a5, a6, a7, 0x32);
}
asm(".global sub_0803E208\n.thumb_set sub_0803E208, AddCannonInventionRecord\n");

void AddMinicannonInventionRecord(int a1, int a2, int a3, int a4, int a5)
{
    struct Unk02028360Pos pos;

    sub_0803DF98(4, &pos);
    sub_0803E088(a1 - pos.unk00, a2 - pos.unk02, 1, 1, 4, 0x63, a3, a4, a5, 0x1e);
}
asm(".global sub_0803E260\n.thumb_set sub_0803E260, AddMinicannonInventionRecord\n");

void AddDeathRayInventionRecord(int a1, int a2, int a3, int a4, int a5, int a6)
{
    struct Unk02028360Pos pos;

    sub_0803DF98(5, &pos);
    sub_0803E088(a1 - pos.unk00, a2 - pos.unk02, a3, a4, 5, 0x63, a5, a6, 0, 0x50);
}
asm(".global sub_0803E2B8\n.thumb_set sub_0803E2B8, AddDeathRayInventionRecord\n");
