#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D1AC.
 * sub_0804D1AC @ 0x0804D1AC
 */

/* The gUnknown_03001470 continuation shape of src/decomp/c_0804E4CC.c and
 * c_0804E334.c, differing in what it does once the delta has moved: it records
 * the delta in unk28 and forwards to CopyFigurePose200.
 *
 * `e` is `d >> 4` and shares agbcc's `(d << 16) >> 20` with the u16 narrowing
 * of `d` itself -- one shifted value feeding two extracts, which is why no
 * separate `lsrs` for `e` appears. */
void FigureTileHook_CruiserVariant2(s16 a, u16 *p)
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
    e = d >> 4;
    p[2] = (p[2] & 0xfc00) + oam.tileNum;

    if (d != gUnknown_03001470[a].unk28
        && a == gUnknown_02029A10[g].entries[h].unk18)
    {
        gUnknown_03001470[a].unk28 = d;
        CopyFigurePose200(g, h, e);
    }
}
asm(".global sub_0804D1AC\n.thumb_set sub_0804D1AC, FigureTileHook_CruiserVariant2\n");
