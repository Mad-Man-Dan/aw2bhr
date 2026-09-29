#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B814.
 * sub_0805B814 @ 0x0805B814, sub_0805B8F4 @ 0x0805B8F4
 */

struct CellXY
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};
struct Unk40D8Bits
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 6;
               u8 unk09_6 : 2;
};
struct Unk5B980Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void sub_0805B814(u16 *p)
{
    struct CellXY a;
    struct CellXY b;
    u16 ax;
    u16 ay;
    u16 x;
    u16 y;
    u8 d1;
    u8 d2;

    if (sub_0805BA34(p[0], p[1], (u16 *)&a) == 0)
        return;

    d1 = 0;
    ax = a.x;
    x = p[0];
    if (ax != x)
    {
        d1 = x - ax + 3;
        y = p[1];
    }
    else
    {
        ay = a.y;
        y = p[1];
        if (ay != y)
            d1 = ay - y + 2;
    }

    gUnknown_030045C8 = d1;

    if (sub_0805BC7C(x, y, (u16 *)&b) == 1)
    {
        d2 = 0;
        if (b.x != p[0])
            d2 = p[0] - b.x + 3;
        else if (b.y != p[1])
            d2 = b.y - p[1] + 2;

        ((struct Unk40D8Bits *)gUnknown_030040D8)->unk09_6 -= 2;
        sub_0805D648(p[0], p[1], 8, d1, d2);
    }
    else if (d1 != 0)
    {
        ((struct Unk40D8Bits *)gUnknown_030040D8)->unk09_6 -= 1;
        sub_0805D648(p[0], p[1], 8, d1, 0);
    }
}

u8 sub_0805B8F4(u16 *outp)
{
    struct CellXY *out;
    struct Unk5B980Cell *p;
    int limit;

    out = (struct CellXY *)outp;
    p = (struct Unk5B980Cell *)gUnknown_03003F20;
    limit = GetUnitMovementBudget((struct Unit *)gUnknown_030040D8);
    out->x = 0x270F;

    while (p->v != -1)
    {
        if (p->v <= limit)
        {
            if (sub_0805BB8C(p->x, p->y) == 1)
            {
                out->x = p->x;
                out->y = p->y;
                return 1;
            }
        }
        p++;
    }

    p = (struct Unk5B980Cell *)gUnknown_03003F20;

    if (out->x == 0x270F)
        AiPopLastNearestCandidate(p, (u16 *)out);

    return 0;
}
