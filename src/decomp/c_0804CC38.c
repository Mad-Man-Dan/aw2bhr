#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804CC38.
 * sub_0804CC38 @ 0x0804CC38
 */

/* Twin of SubmarinePart_Init -- see the note there. The two differ in exactly one
 * immediate (the tile base, 0x80 here against 0x50) and one pool symbol (the
 * continuation, LanderPart_StreamHook against SubmarinePart_StreamHook). Nothing else in 332 bytes.
 */
void LanderPart_Init(void)
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
    oam.tileNum = gUnknown_0300453C * 0x100 + 0x80;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;
    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetSlotSpritePosition(gUnknown_03001FBC,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
    SetSlotSpriteHook(gUnknown_03001FBC, (u32)LanderPart_StreamHook);
}
asm(".global sub_0804CC38\n.thumb_set sub_0804CC38, LanderPart_Init\n");
