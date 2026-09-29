#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013D7C.
 * sub_08013D7C @ 0x08013D7C
 */

/* Draws the NUL-terminated string s into a 32-wide BG tilemap at column x, row
 * y, two tile rows per glyph, OR'ing attr into each Shift-JIS tile. "\n" is
 * skipped; "C1"/"C2" draw the unit icon of gUnknown_030040D8->unk07[0]/[1];
 * "M<n>" draws a 2x2 block of tiles 0xA0B0 + n * 4.
 *
 * The tilemap stores are written as direct addresses, `(tilemap + pos + i)[k]`,
 * not through a `dst` local: in address context agbcc builds `i * 2 +
 * (pos * 2 + tilemap)`, operand order the ROM has, and a local reverses it and
 * shifts the allocation of the whole Shift-JIS branch. The same holds for the
 * table read, `tbl[idx]` / `(tbl + idx)[k]`. */
void DrawShiftJisText(int x, int y, u16 *tilemap, u8 *s, u16 attr)
{
    struct Unit *e;
    u16 *tbl;
    u8 *t;
    u16 c;
    u16 key;
    u16 idx;
    u16 n;
    u16 i;
    u16 pos;
    int k;

    i = 0;
    pos = x + y * 32;
    while (*s != 0)
    {
        if (*s == '\n')
        {
            t = s + 1;
            if (*t == 0)
                break;
            s = t;
            continue;
        }
        if (s[0] == 'C' && s[1] == '1')
        {
            e = &gUnits[gUnknown_030040D8->unk07[0]];
            if (e->hp != 0)
                n = Div(e->hp - 1, 10) + 1;
            else
                n = 0;
            WriteUnitTileQuad(tilemap + (x + i) + y * 32, e->type,
                         gUnknown_03003F2C, e->unk07, 0, n, 0, 0);
            i += 2;
            s += 2;
            continue;
        }
        if (s[0] == 'C' && s[1] == '2')
        {
            e = &gUnits[gUnknown_030040D8->unk07[1]];
            if (e->hp != 0)
                n = Div(e->hp - 1, 10) + 1;
            else
                n = 0;
            WriteUnitTileQuad(tilemap + (x + i) + y * 32, e->type,
                         gUnknown_03003F2C, e->unk07, 0, n, 0, 0);
            i += 2;
            s += 2;
            continue;
        }
        if (s[0] == 'M')
        {
            (tilemap + pos + i)[0] = s[1] * 4 + 0xA0B0;
            (tilemap + pos + i)[1] = s[1] * 4 + 0xA0B1;
            (tilemap + pos + i)[32] = s[1] * 4 + 0xA0B2;
            (tilemap + pos + i)[33] = s[1] * 4 + 0xA0B3;
            i += 2;
            s += 2;
            continue;
        }
        c = s[1] | (s[0] << 8);
        key = s[0] | (s[1] << 8);
        s += 2;
        idx = ((c - 0x8140) & 0xFFF8) * 2 + (c & 7);
        k = 8;
        if (c <= 0x823F)
            tbl = gUnknown_0808F380[0];
        else
        {
            idx -= 0x200;
            if (c <= 0x833F)
                tbl = gUnknown_0808F380[1];
            else
            {
                idx -= 0x200;
                if (c <= 0x8397)
                    tbl = gUnknown_0808F380[2];
                else
                {
                    k = 1;
                    tbl = gUnknown_0808FC8C;
                    do
                    {
                        if (key == *tbl)
                            break;
                        tbl += 3;
                    } while (tbl < gUnknown_0809091C);
                    tbl++;
                    idx = 0;
                }
            }
        }
        (tilemap + pos + i)[0] = attr | tbl[idx];
        (tilemap + pos + i)[32] = attr | (tbl + idx)[k];
        i++;
    }
}
asm(".global sub_08013D7C\n.thumb_set sub_08013D7C, DrawShiftJisText\n");
