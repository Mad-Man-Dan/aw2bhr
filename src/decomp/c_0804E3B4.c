#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E3B4.
 * sub_0804E3B4 @ 0x0804E3B4
 */

/* Twin of sub_0804D928 -- byte-identical apart from the tile base (0xc0 here,
 * 0x60 there) and the continuation symbol. See that function's comment and the
 * "Large functions" section of docs/agbcc-codegen.md.
 */

void sub_0804E3B4(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;

    sub_0801566C(gUnknown_03001FBC, (struct UnkVec *)&oam);
    gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100 + 0xc0;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;
    sub_08015608(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    sub_08015928(gUnknown_03001FBC, (u32)BattleshipPart2_StreamHook);
}
