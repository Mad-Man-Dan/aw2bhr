#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D4E8.
 * sub_0804D4E8 @ 0x0804D4E8, sub_0804D5D8 @ 0x0804D5D8
 */

/* gUnknown_08136070 is a -fforce-addr address constant holding &gUnknown_03001FBC
 * -- the same value CruiserVariant2Figure_Loop's gUnknown_0813606C holds. Honest spelling. */
void CruiserFigure_Loop(void)
{
    u16 c;
    u16 e;
    u16 t;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    t = sub_080156C4(gUnknown_03001FBC);

    if (t == 2)
    {
        if (gUnknown_020296B0[c].unk0c[gUnknown_020296B0[c].unk18] != 0xff)
        {
            SpawnThirdEffectAndProjectile(c, gUnknown_02029A10[c].entries[e].unk1e);
            gUnknown_02029A10[c].entries[e].unk1e++;
            PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[c][1]]
                                          .unk0c[gUnknown_03004580[c][2] - 1]
                                          [gUnknown_020296B0[c].unk1a & 1]);
            gUnknown_020296B0[c].unk1a++;
        }

        gUnknown_020296B0[c].unk18++;
    }

    sub_0804CA98(c, e, gUnknown_03001FBC);
    RidePartOnFigure(c, e, gUnknown_03001FBC);
    StepFigureSlide(c, e);
}
asm(".global sub_0804D4E8\n.thumb_set sub_0804D4E8, CruiserFigure_Loop\n");

/* The gUnknown_03001470 continuation shape of src/decomp/c_0804D1AC.c, with a
 * threshold on `e` selecting between two loaders and an extra 0x20 on the tile
 * number. gUnknown_08136074 is a -fforce-addr address constant holding
 * &gUnknown_03001470 (dereferenced in baserom.gba), so the honest spelling is
 * just the array.
 *
 * The `(u16)` on `oam.tileNum + 0x20` is load-bearing and is NOT a width: it
 * blocks fold from reassociating the constant onto the masked `p[2]`. Without
 * it the arm is `((p[2] & 0xfc00) + 0x20) + tileNum` -- same length, wrong
 * three bytes. */
void FigureTileHook_Cruiser(s16 a, u16 *p)
{
    struct OamData oam;
    u16 d;
    u16 e;
    u16 g;
    u16 h;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);
    d = (p[2] - oam.tileNum) & 0x3ff;
    g = gUnknown_03001470[a].unk30;
    h = gUnknown_03001470[a].unk34;
    e = d >> 5;

    if (e > 10)
        p[2] = (p[2] & 0xfc00) + (u16)(oam.tileNum + 0x20);
    else
        p[2] = (p[2] & 0xfc00) + oam.tileNum;

    if (d != gUnknown_03001470[a].unk28
        && a == gUnknown_02029A10[g].entries[h].unk18)
    {
        gUnknown_03001470[a].unk28 = d;

        if (e > 10)
            CopyFigurePose400Upper(g, h, e);
        else
            CopyFigurePose400(g, h, e);
    }
}
asm(".global sub_0804D5D8\n.thumb_set sub_0804D5D8, FigureTileHook_Cruiser\n");
