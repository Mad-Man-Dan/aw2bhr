#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080078E4.
 * sub_080078E4 @ 0x080078E4
 */

/*
 * DesignRoomBuildItemList -- build the design ring's list of entries at
 * gUnknown_0200B224.
 *
 * a picks the list: 0 walks the terrain template gUnknown_08488810, anything
 * else the unit template gUnknown_08488856. Both are halfwords terminated by
 * 0xFF, and every entry becomes two halfwords of output.
 *
 * Terrain template: an entry with any of bits 5-7 set is a property. Its low
 * five bits select one of five kinds (8, 6, 14, 10 and 11 map to 0 to 4; any
 * other value leaves the previous kind in place), and the two output halfwords
 * come from gUnknown_084887AC row (b + kind * 5) while the template's next
 * halfword is skipped. Any other entry is copied out together with the
 * halfword that follows it.
 *
 * Unit template: b is the army number, and (b - 1) << 6 puts it in bits 6-7 of
 * every id except 0x19, which is left as it is. Each id is written twice.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The cases are listed 8, 6, 14, 10, 11 -- the order of the values they
 *     assign, not ascending label order. The compiler emits case bodies in
 *     source order, so reordering them changes the jump table and the layout
 *     of the blocks.
 *   - `b = (b - 1) << 6;` overwrites the parameter instead of using a new
 *     local. That keeps the shifted value in b's own register and also stops
 *     the compiler from copying the loop's end test to the top of the loop.
 *   - The row's two halfwords are read through the bound pointer `e`, as e[0]
 *     and e[1]. Subscripting the array twice recomputes the whole address.
 *   - `v` is an `int`. As a u16 the `v |= b` would have to be truncated again,
 *     which the original does not do.
 */

void DesignRoomBuildItemList(int a, int b)
{
    u16 *q;
    const u16 *s;
    const u16 *e;
    int k;
    int v;

    k = 0;
    q = (u16 *)gUnknown_0200B224;
    if (a == 0)
    {
        s = gUnknown_08488810;
        while (*s != 0xFF)
        {
            v = *s++;
            if (v & 0xE0)
            {
                switch (v & 0x1F)
                {
                case 8:
                    k = 0;
                    break;
                case 6:
                    k = 1;
                    break;
                case 14:
                    k = 2;
                    break;
                case 10:
                    k = 3;
                    break;
                case 11:
                    k = 4;
                    break;
                }
                e = &gUnknown_084887AC[(b + k * 5) * 2];
                *q++ = e[0];
                *q++ = e[1];
                s++;
            }
            else
            {
                *q++ = v;
                *q++ = *s++;
            }
        }
    }
    else
    {
        s = gUnknown_08488856;
        b = (b - 1) << 6;
        while ((v = *s++) != 0xFF)
        {
            if (v != 0x19)
                v |= b;
            *q++ = v;
            *q++ = v;
        }
    }
}
asm(".global sub_080078E4\n.thumb_set sub_080078E4, DesignRoomBuildItemList\n");
