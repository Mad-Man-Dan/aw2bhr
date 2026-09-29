#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057048.
 * sub_08057048 @ 0x08057048
 */

/* DrawTileBlock4x4A @ 0x08057048.
 *
 * WAVE 37 (W37-J3): un-parked. The wave-33 note in docs/agbcc-codegen.md said
 * this needed gUnknown_085D6A48 retyped in the header; it does not. The
 * row-struct cast gives the `ldrh [r,#2]` displacement but loads the base pool
 * word last when written in place, and hoists it ahead of gUnknown_085538A2
 * when bound in a statement of its own. Creating the pseudo INSIDE the
 * subscript with a comma operator gives the displacement AND the ROM's
 * outermost-first pool order (085538A2, 085D6A48, 03004580) at once. The same
 * lever closed DrawTileBlock5x5B, which reads the same column of the same table.
 */

struct Unk8057048
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
};
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x18 - 0x04];
};

void DrawTileBlock4x4A(u16 a1, u16 a2)
{
    struct Unk85D6A48Row *tbl;
    struct Unk8057048 s;

    s.unk00 = gUnknown_0855388C[a1];
    s.unk02 = 4;
    s.unk04 = 4;
    s.unk06 = gUnknown_085538A2[
        (tbl = (struct Unk85D6A48Row *)gUnknown_085D6A48,
         tbl)[gUnknown_03004580[a2][1]].unk02 * 2 + a2];
    s.unk08 = 0;
    s.unk0a = 0;
    DrawTileBlockA(&s);
    gUnknown_03004534 = 1;
}
asm(".global sub_08057048\n.thumb_set sub_08057048, DrawTileBlock4x4A\n");
