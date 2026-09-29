#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080566C8.
 * sub_080566C8 @ 0x080566C8
 */

/* LoadFigureSpriteSets -- MATCHED, wave 60 (W60-H).  The per-side setup pass: it copies
 * five columns of gUnknown_03004580 into eight two-element stack arrays, picks
 * a per-side variant out of gUnknown_08552118 both ways round, installs the
 * unit record sub_08057D58 returns (or the gUnknown_085D64A8 fallback) into
 * gUnknown_02029BA8 and Decompresses its tiles, then fills five slots of each
 * side's gUnknown_02029710 / gUnknown_020298E0 offset rows -- the two sides
 * CROSSED, and with the first member of each (x, y) pair negated for one of
 * them.
 *
 * gUnknown_085D6A48's 24-byte rows are STRUCTS, not `u16[12]`: columns 1 and 10
 * stay in the `ldrh` displacement instead of being reassociated onto the base.
 * The global keeps its `u16 [][12]` declaration (wave 17 recorded why), so the
 * row shape is spelled here.  The WRAPPER spelling `((W *)g)->unk00[x].m` is
 * what matches, not the pointer subscript `((Row *)g)[x].m` and not the
 * in-loop binding local c_080579B8.c uses: all three keep the offset in the
 * displacement, but only the wrapper emits the base's pool `ldr` BEFORE the
 * index chain, which is what the ROM does at every one of the 22 sites.  See
 * the wave-60 chapter in docs/agbcc-codegen.md. */
struct Unk085D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x10];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 filler_16[0x02];
};
struct Unk085D6A48Obj
{
    /* 0x00 */ struct Unk085D6A48Row unk00[1];
};
/* gUnknown_085D6EC8's 20-byte rows carry three WORDS the ROM reaches with the
 * offset hoisted onto the base (`adds rB,rSym,#0; adds rB,#0xc`, zero `ldr`
 * displacement) off ONE bare pool word.  The global keeps its `u16 [][10]`
 * declaration; c_0805521C.c reads the same words through a `*(void **)&g[a][2]`
 * cast, but that spelling folds the +0xc onto the SYMBOL here and costs a pool
 * word per column, so this one needs the row struct too. */
struct Unk085D6EC8Row /* 0x14 */
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ void *unk08;
    /* 0x0c */ void *unk0c;
    /* 0x10 */ void *unk10;
};
struct Unk085D6EC8Obj
{
    /* 0x00 */ struct Unk085D6EC8Row unk00[1];
};
/* The record sub_08057D58 returns, and the one gUnknown_085D64A8 holds a table
 * of.  Only LoadFigureSpriteSets dereferences it, so the tag lives here.  unk00's
 * 8-byte stride and unk18's 12-byte one are both measured off the ROM's index
 * chains; the extents are the distance between the two members and 1. */
struct Unk080566C8Rec /* 0x0c */
{
    /* 0x00 */ void *unk00;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07[3];
    /* 0x0a */ u8 unk0a;
    /* 0x0b */ u8 filler_0b[0x01];
};
struct Unk080566C8Unit
{
    /* 0x00 */ u8 *unk00[3][2];
    /* 0x18 */ struct Unk080566C8Rec unk18[1];
};

void LoadFigureSpriteSets(int a1)
{
    u16 a[2];
    u16 b[2];
    u16 c[2];
    u16 d[2];
    u16 e[2];
    u16 f[2];
    u16 g[2];
    u16 h[2];
    u16 i;
    u16 j;
    struct Unk080566C8Unit *p;

    for (i = 0; i < 2; i++)
    {
        a[i] = ((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[gUnknown_03004580[i][1]].unk00;
        b[i] = gUnknown_03004580[i][2];
        c[i] = gUnknown_03004580[i][0];
        d[i] = gUnknown_03004580[i][1];
        g[i] = gUnknown_03004580[i][3];
        h[i] = gUnknown_03004580[i][4];
        e[i] = ((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[d[i]].unk02;

        if (d[i] <= 1)
            e[i] = gUnknown_08555850[g[i]].unk00;
    }

    f[0] = gUnknown_08552118[e[0]][e[1]];
    f[1] = gUnknown_08552118[e[1]][e[0]];
    gUnknown_020296B0[0].unk1c = f[0];
    gUnknown_020296B0[1].unk1c = f[1];
    b[0] = sub_08056D70(d[0], f[0], b[0]);
    b[1] = sub_08056D70(d[1], f[1], b[1]);

    for (i = 0; i < 2; i++)
    {
        p = sub_08057D58(d[i], g[i], gUnknown_08562128[h[i]]);

        if (p == 0)
            p = gUnknown_085D64A8[0].unk00[gUnknown_08562128[h[i]] + d[i] * 5];

        gUnknown_02029BA8[i].unk00 = p->unk18[b[i]].unk00;
        gUnknown_02029BA8[i].unk04 = (void *)(u32)p->unk18[b[i]].unk04;
        gUnknown_02029BA8[i].unk08 = (void *)(u32)p->unk18[b[i]].unk05;
        gUnknown_02029BA8[i].unk0c = (void *)(u32)p->unk18[b[i]].unk06;
        gUnknown_02029BA8[i].unk10 = (void *)(u32)p->unk18[b[i]].unk07[f[i]];
        gUnknown_02029BA8[i].unk14 = (void *)(u32)p->unk18[b[i]].unk0a;
        Decompress(p->unk00[b[i]][0], gUnknown_08553850[i]);
        gUnknown_02029BA8[i].unk18[0] = gUnknown_08553850[i];
        gUnknown_02029BA8[i].unk18[1] = gUnknown_08553850[i];
        gUnknown_02029700[i][0] = (void *)gUnknown_085D70A8[a[i]][b[i]].unk0c[f[i]];
        gUnknown_02029700[i][1] = (void *)gUnknown_085D70A8[a[i]][b[i]].unk08;
        gUnknown_02029A00[i][0] = (void *)gUnknown_085D7768[a[i]][b[i]].unk0c[f[i]];
        gUnknown_02029A00[i][1] = (void *)gUnknown_085D7768[a[i]][b[i]].unk08;
        gUnknown_02029710[i].unk04 = (void *)gUnknown_08553D80.unk00[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[i]].unk14][c[i]].unk0c;
        gUnknown_02029710[i].unk08 = (void *)gUnknown_08553D80.unk00[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[i]].unk14][c[i]].unk08;
        gUnknown_02029710[i].unk16 = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[i]].unk14].unk00;
        gUnknown_02029710[i].unk18 = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[i]].unk14].unk02[i];
        gUnknown_02029710[i].unk1a = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[i]].unk14].unk06;
        gUnknown_020297C0[i].unk18 = ((struct Unk085D6EC8Obj *)gUnknown_085D6EC8)->unk00[a[i]].unk0c;
        gUnknown_020297C0[i].unk1c = ((struct Unk085D6EC8Obj *)gUnknown_085D6EC8)->unk00[a[i]].unk10;
        gUnknown_020297C0[i].unk20 = ((struct Unk085D6EC8Obj *)gUnknown_085D6EC8)->unk00[a[i]].unk08;
    }

    for (j = 0; j < 5; j++)
    {
        gUnknown_02029710[0].unk26[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk0c;
        gUnknown_02029710[0].unk30[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk0e;
        gUnknown_02029710[0].unk44[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk12;
        gUnknown_02029710[0].unk4e[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk14;
        gUnknown_02029710[1].unk26[j] = -gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk0c;
        gUnknown_02029710[1].unk30[j] = -gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk0e;
        gUnknown_02029710[1].unk44[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk12;
        gUnknown_02029710[1].unk4e[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk14;
        gUnknown_020298E0[1].unk58[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk18;
        gUnknown_020298E0[1].unk62[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk1a;
        gUnknown_020298E0[1].unk6c[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk1e;
        gUnknown_020298E0[1].unk76[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[0]].unk14].unk20;
        gUnknown_020298E0[0].unk58[j] = -gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk18;
        gUnknown_020298E0[0].unk62[j] = -gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk1a;
        gUnknown_020298E0[0].unk6c[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk1e;
        gUnknown_020298E0[0].unk76[j] = gUnknown_08553C18[((struct Unk085D6A48Obj *)gUnknown_085D6A48)->unk00[a[1]].unk14].unk20;
    }

    sub_08056D8C(0, d[0], b[0]);
    sub_08056D8C(1, d[1], b[1]);
}
asm(".global sub_080566C8\n.thumb_set sub_080566C8, LoadFigureSpriteSets\n");
