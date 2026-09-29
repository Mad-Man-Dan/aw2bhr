#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804CD84.
 * LanderPart_Loop @ 0x0804CD84
 */

/* MATCHED. Byte-for-byte the same function as SubmarinePart_Loop -- one C body, two
 * addresses. Read that one; the `pal` local and the struct spelling of
 * gUnknown_020298E0 are the two load-bearing choices. */
void LanderPart_Loop(void)
{
    struct OamData oam;
    u16 side;
    u16 slot;
    s16 t;
    u16 pal;

    side = gUnknown_03001470[gUnknown_03001FBC].unk30;
    slot = gUnknown_03001470[gUnknown_03001FBC].unk34;

    RidePartOnFigure(side, slot, gUnknown_03001FBC);
    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    t = gUnknown_020298E0[side].unk26[slot] + gUnknown_020298E0[side].unk30[slot];
    pal = gUnknown_08553B40[side][t];
    oam.paletteNum = pal;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
}

asm(".global sub_0804CD84\n.thumb_set sub_0804CD84, LanderPart_Loop\n");
