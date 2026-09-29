#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D928.
 * sub_0804D928 @ 0x0804D928
 */

/* Rebuilds the OBJ attributes of the sprite belonging to the current
 * gUnknown_03001470 slot, then arms a continuation.
 *
 * Twin of BattleshipPart2_Init, which differs only in the tile base (0xc0 vs 0x60) and
 * the continuation symbol. See "Large functions" in docs/agbcc-codegen.md for
 * the three source spellings this function turns on:
 *   - `pal = tbl[...]; oam.paletteNum = pal;` -- a bitfield store narrows a
 *     table `ldrh` to `ldrb` unless the loaded value is bound to a local first.
 *   - `* 0x100` and not `<< 8` -- the multiply is shortened to HImode by the C
 *     front end, so it keeps a real memory operand and is not CSEd against the
 *     value loaded for the hFlip statement two statements earlier.
 *   - the priority index reads BOTH globals directly; binding either one to a
 *     local lets combine drop the `lsl #16; lsr #16` zero-extension.
 */

void CruiserPart2_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);
    gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100 + 0x60;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;
    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetSlotSpriteHook(gUnknown_03001FBC, (u32)CruiserPart2_StreamHook);
}
asm(".global sub_0804D928\n.thumb_set sub_0804D928, CruiserPart2_Init\n");
