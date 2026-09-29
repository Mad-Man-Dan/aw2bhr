#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080553C8.
 * sub_080553C8 @ 0x080553C8
 */

/* A file-local WRAPPER view of gUnknown_085D6A48 (declared `u16 [][12]` in
 * include/unknown-globals.h, and left that way). The 10-byte leading filler is
 * what puts the +0xa on the BASE pseudo -- `adds r3, #0xa`, computed once and
 * shared by both reads -- instead of on the column subscript. The row extent is
 * not proved; only the 0x18 stride and the 0x0a column offset are. */
struct Tbl85D6A48
{
    u8 filler_00[0x0a];
    u16 cells[8][12];
};

void PlanBattleAnim(void)
{
    u16 a;
    u16 b;
    u16 s;

    a = gUnknown_03004580[0][3];
    b = gUnknown_03004580[1][3];
    gUnknown_03004580[0][3] = a & 0x1F;
    gUnknown_03004580[1][3] = b & 0x1F;
    gUnknown_030045A8[0] = GetCellCountry(a);
    gUnknown_030045A8[1] = GetCellCountry(b);

    gUnknown_03004500[0] = gUnknown_03004580[0][0];
    gUnknown_03004500[1] = gUnknown_03004580[1][0];
    gUnknown_03004580[0][0] = gUnknown_08553846[gUnknown_03004580[0][0]];
    gUnknown_03004580[1][0] = gUnknown_08553846[gUnknown_03004580[1][0]];

    if (gUnknown_03004504.bit0)
    {
        if (gUnknown_03004580[0][1] == 0x12)
            gUnknown_03004580[0][1] = 3;

        if (gUnknown_03004580[1][1] == 0x12)
            gUnknown_03004580[1][1] = 3;
    }

    if (gUnknown_03004580[0][1] == 0x17 && (gUnknown_03004528[0][1] & 0x20))
        gUnknown_03004580[0][1] = 0x11;

    if (gUnknown_03004580[1][1] == 0x17 && (gUnknown_03004528[1][1] & 0x20))
        gUnknown_03004580[1][1] = 0x11;

    if (gUnknown_03004580[0][5] != gUnknown_03004580[0][6])
        gUnknown_02028E4C[5] = 0;

    if (gUnknown_03004580[1][5] != gUnknown_03004580[1][6])
        gUnknown_02028E4C[0xd] = 0;

    gUnknown_02029BE8[0] = ((struct Tbl85D6A48 *)gUnknown_085D6A48)
                               ->cells[gUnknown_03004580[0][1]][gUnknown_03004580[0][2]];
    gUnknown_02029BE8[1] = ((struct Tbl85D6A48 *)gUnknown_085D6A48)
                               ->cells[gUnknown_03004580[1][1]][gUnknown_03004580[1][2]];

    gUnknown_02029B78[0] = gUnknown_03004580[0][5];
    gUnknown_02029B78[1] = gUnknown_03004580[1][5];
    gUnknown_03004580[0][5] = gUnknown_03004580[0][5] ? Div(gUnknown_03004580[0][5] - 1, 10) + 1 : 0;
    gUnknown_03004580[1][5] = gUnknown_03004580[1][5] ? Div(gUnknown_03004580[1][5] - 1, 10) + 1 : 0;

    gUnknown_02029B7C[0] = gUnknown_03004580[0][6];
    gUnknown_02029B7C[1] = gUnknown_03004580[1][6];
    gUnknown_03004580[0][6] = gUnknown_03004580[0][6] ? Div(gUnknown_03004580[0][6] - 1, 10) + 1 : 0;
    gUnknown_03004580[1][6] = gUnknown_03004580[1][6] ? Div(gUnknown_03004580[1][6] - 1, 10) + 1 : 0;

    sub_08057164(gUnknown_03004580[0][5], gUnknown_03004580[0][6], 0);
    sub_08057164(gUnknown_03004580[1][5], gUnknown_03004580[1][6], 1);

    sub_080555F0(0, gUnknown_0300450C);
    sub_080555F0(1, gUnknown_0300450C);

    BuildShotLists(gUnknown_02029BE8[0], gUnknown_02029BE8[1]);
    sub_08055940(gUnknown_02029BE8[0], gUnknown_02029BE8[1]);
    BuildDeathLists(gUnknown_02029BE8[0], gUnknown_02029BE8[1]);

    s = gUnknown_0300450C ^ 1;

    if (gUnknown_03004580[s][1] == 0x15 && gUnknown_03004580[s][2] == 1)
        gUnknown_020296B0[s].unk1e[0] += 0x20;
}
asm(".global sub_080553C8\n.thumb_set sub_080553C8, PlanBattleAnim\n");
