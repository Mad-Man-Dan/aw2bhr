#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805198C.
 * sub_0805198C @ 0x0805198C, sub_08051A44 @ 0x08051A44, sub_08051B30 @ 0x08051B30
 */

void SpawnFireEffect(u16 a, u16 b)
{
    u16 t;
    int f;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    t = gUnknown_085D6A48[gUnknown_03004580[a][1]][0];

    if (gUnknown_085D6EC8[t][0] != 0)
    {
        f = gUnknown_08552178[a][b] * 7 + 3;
        gUnknown_020297C0[a].unk02[b] = sub_08015410(gUnknown_0855368C, 1,
            gUnknown_020297C0[a].unk18, gUnknown_020297C0[a].unk20, f);
    }
}
asm(".global sub_0805198C\n.thumb_set sub_0805198C, SpawnFireEffect\n");

void FireEffect_Init(void)
{
    struct OamData oam;
    int tile;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
    oam.hFlip = gUnknown_0300453C ^ 1;
    oam.paletteNum = 8;
    tile = gUnknown_020297C0[gUnknown_0300453C].unk00;
    oam.tileNum = tile;
    oam.priority = 3;
    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetEffectScreenPosition(gUnknown_0300453C, gUnknown_03001FBC,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
}
asm(".global sub_08051A44\n.thumb_set sub_08051A44, FireEffect_Init\n");

void FireEffect_Loop(void)
{
    u16 a;
    u16 b;

    a = gUnknown_03001470[gUnknown_03001FBC].unk30;
    b = gUnknown_03001470[gUnknown_03001FBC].unk34;

    SetEffectScreenPosition(a, gUnknown_03001FBC,
        gUnknown_02029A10[a].entries[b].x,
        gUnknown_02029A10[a].entries[b].y);

    if (gUnknown_020297C0[a].unk0c[b] == 1)
    {
        if (gUnknown_03001FBC != -1 && IsSlotScriptActiveAt(gUnknown_03001FBC))
            sub_080156E8(gUnknown_03001FBC, gUnknown_020297C0[a].unk1c);

        gUnknown_020297C0[a].unk0c[b] = 0;
    }
}
asm(".global sub_08051B30\n.thumb_set sub_08051B30, FireEffect_Loop\n");
