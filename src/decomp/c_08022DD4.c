#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022DD4.
 * sub_08022DD4 @ 0x08022DD4
 */

/* The gUnknown_08499590 map header seen through the members this function
 * needs.  Declared LOCALLY and cast onto the `u8 *` symbol, per the rule in
 * include/unknown-globals.h: only a COMPONENT_REF keeps agbcc from folding
 * 0x12 into a load displacement or reassociating the 0x417A row table.
 * Wave 56, W56-H. */
void DrawMapCursorSpriteUnused(s16 a1, s16 a2, s16 a3)
{
    struct Map *m;
    struct UnkVec vec;
    int v;
    int x;
    int y;
    int row;

    v = 0;
    *(long long *)&vec = 0;
    ((struct OamData *)&vec)->tileNum = 0x365;
    ((struct OamData *)&vec)->paletteNum = 1;

    switch (a3)
    {
    case 0:
        v = (u32)gGameClock % 33;
        m = gMap;
        y = (a2 + m->scrollY) >> 4;
        row = m->rowOffset[y];
        x = (a1 + m->scrollX) >> 4;
        if (m->unit[row + x] == 0 && IsOwnFactoryCell(x, y) != 0)
        {
            a3 = 4;
            if (v <= 4)
                v = 0xA;
            else if (v <= 8)
                v = 0xB;
            else if (v <= 0x1C)
                v = 0xC;
            else
                v = 0xB;
        }
        else
        {
            if (v <= 4)
                v = 0;
            else if (v <= 8)
                v = 1;
            else if (v <= 0x1C)
                v = 2;
            else
                v = 1;
        }
        break;
    case 1:
        v = (u32)gGameClock % 33;
        if (v <= 4)
            v = 0;
        else if (v <= 8)
            v = 1;
        else if (v <= 0x1C)
            v = 2;
        else
            v = 1;
        break;
    case 2:
        v = (u32)gGameClock % 20;
        if (v <= 4)
            v = 6;
        else if (v <= 9)
            v = 7;
        else if (v <= 0xE)
            v = 8;
        else
            v = 9;
        break;
    case 3:
        v = (u32)gGameClock % 28;
        if (v <= 5)
            v = 3;
        else if (v <= 8)
            v = 4;
        else if (v <= 0x19)
            v = 5;
        else
            v = 4;
        break;
    case 5:
        v = (u32)gGameClock % 33;
        if (v <= 4)
            v = 0xA;
        else if (v <= 8)
            v = 0xB;
        else if (v <= 0x1C)
            v = 0xC;
        else
            v = 0xB;
        break;
    case 6:
        v = (u32)gGameClock % 33;
        if (v <= 4)
            v = 0xA;
        else if (v <= 8)
            v = 0xB;
        else if (v <= 0x1C)
            v = 0xC;
        else
            v = 0xB;
        break;
    case 7:
        v = (u32)gGameClock % 26;
        if (v <= 0xF)
            v = 0;
        else if (v <= 0x12)
            v = 1;
        else if (v <= 0x16)
            v = 2;
        else
            v = 1;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        v = (u32)gGameClock % 24;
        if (v <= 3)
            v = 3;
        else if (v <= 5)
            v = 4;
        else if (v <= 0x15)
            v = 5;
        else
            v = 3;
        v += (a3 - 8) * 3;
        break;
    case 12:
    case 13:
        v = (u32)gGameClock % 24;
        if (v <= 3)
            v = 0xF;
        else if (v <= 5)
            v = 0x10;
        else if (v <= 0x15)
            v = 0x11;
        else
            v = 0x10;
        v += (a3 - 12) * 3;
        break;
    }

    if (a3 > 6)
    {
        gUnknown_03003F58.unk00 = a1;
        gUnknown_03003F58.unk02 = a2;
        gUnknown_03003F58.unk04 = v;
        PutOamHi(gUnknown_03003F58.unk00, (s16)((u16)a2 | 0x400),
                     gUnknown_0848ABF4[gUnknown_03003F58.unk04], 0x365);
    }
    else
    {
        ((struct OamData *)&vec)->tileNum = 0x365;
        ((struct OamData *)&vec)->paletteNum = 1;
        sub_0801C01C(a1, a2, gUnknown_0848AE98[v], vec, 1);
    }
    DrawMapCursorPointerSpriteUnused(a1, a2, a3);
}
asm(".global sub_08022DD4\n.thumb_set sub_08022DD4, DrawMapCursorSpriteUnused\n");
