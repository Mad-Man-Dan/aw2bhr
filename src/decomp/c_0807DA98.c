#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807DA98.
 * sub_0807DA98 @ 0x0807DA98
 */

#include "hardware.h"
/* CoSelect_DrawSprites @ 0x0807DA98, 3816 bytes.
 *
 * The 0x081D9364 pool word is NOT a global: [0x081D9364] = 0x0808F100 = &gSinLut,
 * an agbcc -fforce-addr address constant. Spelling gSinLut honestly reproduces
 * the double load; the promotion carries "rodata": ["0x081D9364"]. Exactly the
 * mechanism unknown-globals.h records for 0x081D946C. (wave 56, W56-B)
 *
 * The parameter is the same object IsCoSelectionUnique/D860/D918 take, seen through a
 * wider window: this function adds +0x2c..+0x40, +0x48, +0x4e, +0x64, +0x66 and
 * +0x6a to the s16 unk4c / u16 unk52 / int unk58,5c,60 those three already model.
 */
struct Unk7DA98
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ u8 filler_44[4];
    /* 0x48 */ u16 unk48;
    /* 0x4a */ u8 filler_4a[2];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[2];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[4];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ volatile int unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ u8 filler_68[2];
    /* 0x6a */ s16 unk6a;
};
/* Defined in src/decomp/c_0807D800.c over its own tag for this same object. */

void CoSelect_DrawSprites(struct Unk7DA98 *p)
{
    int i;
    int j;
    int x;
    int y;
    int a;
    int b;
    int c;
    int d;

    if (p->unk4e != 0)
    {
        sub_08043FD8((0x88 - p->unk38 * 3) & 0x1ff, ((p->unk38 + 0x3c) & 0xff) | 0x400, 0x7a34, 5);
        sub_08043FD8((0xd0 - p->unk38 * 3) & 0x1ff, ((p->unk38 + 0x24) & 0xff) | 0x400, 0x8a58, 5);

        if (p->unk4e > 0)
        {
            sub_08043FD8((0x40 - p->unk2c * 3) & 0x1ff, ((p->unk2c + 0x54) & 0xff) | 0x400, 0x6a10, 5);
            sub_08043FD8((0x118 - p->unk38 * 3) & 0x1ff, ((p->unk38 + 0xc) & 0xff) | 0x400, 0x59ec, 5);
        }
        else if (p->unk4e < 0)
        {
            sub_08043FD8((0x40 - p->unk38 * 3) & 0x1ff, ((p->unk38 + 0x54) & 0xff) | 0x400, 0x6a10, 5);
            sub_08043FD8((-8 - (p->unk2c + 5) * 3) & 0x1ff, ((p->unk2c + 0x71) & 0xff) | 0x400, 0x59ec, 5);
        }

        for (i = 0; i < p->unk64; i++)
        {
            for (j = 1; j <= 2; j++)
            {
                if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52 + j - p->unk4e, gUnknown_03005948[p->unk58]) + p->unk5c])
                {
                    x = p->unk38 * 3 - 0x25;
                    x = j * 0x48 - x;
                    x &= 0x1ff;
                    y = p->unk38 + 0x43;
                    DrawOamObject(i + 0x4a, x, (y - j * 0x18) | 0x400, 0, 0);
                }
            }
        }

        if (p->unk4e > 0)
        {
            for (i = 0; i < p->unk64; i++)
            {
                if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52 - p->unk4e, gUnknown_03005948[p->unk58]) + p->unk5c])
                    DrawOamObject(i + 0x4a, (0x25 - p->unk2c * 3) & 0x1ff, (p->unk2c + 0x43) | 0x400, 0, 0);

                a = p->unk52;
                b = p->unk4e - 3;

                if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(a - b, gUnknown_03005948[p->unk58]) + p->unk5c])
                    DrawOamObject(i + 0x4a, (0xfd - p->unk38 * 3) & 0x1ff, (p->unk38 - 5) | 0x400, 0, 0);
            }
        }
        else if (p->unk4e < 0)
        {
            for (i = 0; i < p->unk64; i++)
            {
                if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52 - p->unk4e, gUnknown_03005948[p->unk58]) + p->unk5c])
                    DrawOamObject(i + 0x4a, (0x25 - p->unk38 * 3) & 0x1ff, (p->unk38 + 0x43) | 0x400, 0, 0);

                if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52 - p->unk4e - 1, gUnknown_03005948[p->unk58]) + p->unk5c])
                    DrawOamObject(i + 0x4a, (-0x23 - (p->unk2c + 5) * 3) & 0x1ff, (p->unk2c + 0x60) | 0x400, 0, 0);
            }
        }

        gUnknown_03002F18 = 0x24 - DivRem(p->unk3c, 4);
        gUnknown_03002B34 = DivRem(p->unk3c, 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xc;
        }

        if (p->unk4e < 0)
        {
            DrawOamObject(0x48,
                         0x18 - Interpolate(4, Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20), 0x10, p->unk4c, 0x10),
                         Interpolate(4, Interpolate(4, 0, 4, DivRem((s16)p->unk48, 0x20), 0x20), 8, p->unk4c, 0x10) + 0x3c,
                         0, 0);
            DrawOamObject(0x49,
                         Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20) + 0x58,
                         0x2c - Interpolate(4, 0, 4, DivRem((s16)p->unk48, 0x20), 0x20),
                         0, 0);
        }
        else
        {
            DrawOamObject(0x48,
                         0x18 - Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20),
                         Interpolate(4, 0, 4, DivRem((s16)p->unk48, 0x20), 0x20) + 0x3c,
                         0, 0);
            DrawOamObject(0x49,
                         Interpolate(4, Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20), 0x10, p->unk4c, 0x10) + 0x58,
                         0x2c - Interpolate(4, Interpolate(4, 0, 4, DivRem((s16)p->unk48, 0x20), 0x20), 8, p->unk4c, 0x10),
                         0, 0);
        }
    }
    else if (p->unk60 != 0)
    {
        sub_08043FD8(0x40, ((0x54 - p->unk30) & 0xff) | 0x400, 0x2980, 5);
        sub_08043FD8(0x88, ((0x3c - p->unk30) & 0xff) | 0x400, 0x39a4, 5);
        sub_08043FD8(0xd0, ((0x24 - p->unk30) & 0xff) | 0x400, 0x49c8, 5);

        for (i = 0; i < p->unk64; i++)
        {
            for (j = 0; j <= 2; j++)
            {
                if (p->unk4c > 13)
                {
                    if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52 + j, gUnknown_03005948[p->unk58]) + p->unk5c])
                        DrawOamObject(i + 0x4a, j * 0x48 + 0x25, ((0x43 - j * 0x18 - p->unk30) & 0xff) | 0x400, 0, 0);
                }
                else
                {
                    if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(gUnknown_03005938[0] + j, gUnknown_03005948[gUnknown_03005938[1]]) + gUnknown_03005938[2]])
                        DrawOamObject(i + 0x4a, j * 0x48 + 0x25, ((0x43 - j * 0x18 - p->unk30) & 0xff) | 0x400, 0, 0);
                }
            }
        }

        gUnknown_03002F18 = p->unk30 + 0x24 - DivRem(Div(p->unk3c, 4), 4);
        gUnknown_03002B34 = DivRem(Div(p->unk3c, 4), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xc;
        }

        p->unk60;

        DrawOamObject(0x48,
                     0x18 - Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20),
                     Interpolate(4, 0, 4, DivRem((s16)p->unk48, 0x20), 0x20) + 0x3c,
                     0, 0);
        DrawOamObject(0x49,
                     Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20) + 0x58,
                     0x2c - Interpolate(4, 0, 4, DivRem((s16)p->unk48, 0x20), 0x20),
                     0, 0);
    }
    else
    {
        sub_08043FD8(0x40, 0x54, 0x2980, 5);
        sub_08043FD8(0x88, 0x43c, 0x39a4, 5);
        sub_08043FD8(0xd0, 0x424, 0x49c8, 5);

        for (i = 0; i < p->unk64; i++)
        {
            for (j = 0; j <= 2; j++)
            {
                if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52 + j, gUnknown_03005948[p->unk58]) + p->unk5c])
                    DrawOamObject(i + 0x4a, j * 0x48 + 0x25, (0x43 - j * 0x18) | 0x400, 0, 0);
            }
        }

        gUnknown_03002F18 = 0x24 - DivRem(Div(p->unk3c, 3), 4);
        gUnknown_03002B34 = DivRem(Div(p->unk3c, 3), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xc;
        }

        DrawOamObject(0x48,
                     0x18 - Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20),
                     Interpolate(4, 0, 4, DivRem(p->unk48, 0x20), 0x20) + 0x3c,
                     0, 0);
        DrawOamObject(0x49,
                     Interpolate(4, 0, 8, DivRem(p->unk48, 0x20), 0x20) + 0x58,
                     0x2c - Interpolate(4, 0, 4, DivRem(p->unk48, 0x20), 0x20),
                     0, 0);
    }

    if ((int)gUnknown_03005944 > 1 && gUnknown_030059C0[p->unk64] != 0)
    {
        DrawOamObject(0x43, 0x38, 0x1c, 0, 2);
        DrawOamObject(0x44, 0x38, 0x54, 0, 2);
    }

    sub_08043C28(p->unk34 + 0xb0, 0xa0, 0x1040, 0, 1);

    if ((u8)IsCoSelectionUnique((struct Unk807D800 *)p) != 0 || p->unk4e != 0 || p->unk60 != 0)
        DrawOamObject(p->unk64 + 0x4a, 0x25, 0x43, 0, 0);

    PutSprite(1, 0x20, 0x1c, gUnknown_0848B6A0, 0);
    DrawOamObject(0x67, 0xb9, 0x86, 0, 0);
    DrawOamObject(0x62, 0xb9, 0x8e, 0, 0);
    DrawOamObject(0x65, 0xb9, 0x96, 0, 0);

    for (i = 0; i < p->unk64; i++)
        sub_08043B60(0x2a, i * 0x10 + 0x68, (i * 0xc + 0x3c0) | 0x9000, 0);

    if (i < sub_0803BD14())
    {
        if (p->unk66 > 0 && p->unk4c <= 8)
        {
            SetObjAffine(0,
                         Div(gSinLut[0x40] * 16, Interpolate(0, 0x10, 0x100, p->unk4c, 8) != 0 ? Interpolate(0, 0x10, 0x100, p->unk4c, 8) : 2),
                         Div(-gSinLut[0] * 16, 0x100),
                         Div(gSinLut[0] * 16, Interpolate(0, 0x10, 0x100, p->unk4c, 8) != 0 ? Interpolate(0, 0x10, 0x100, p->unk4c, 8) : 2),
                         Div(gSinLut[0x40] * 16, 0x100));
            sub_08043B60(0x2a, (i * 0x10 + 0x68) | 0x100, (i * 0xc + 0x3c0) | 0x9000, 0);
        }
        else
        {
            sub_08043B60(0x2a, i * 0x10 + 0x68, (i * 0xc + 0x3c0) | 0x9000, 0);
        }
    }

    if (i + 1 < sub_0803BD14() && p->unk66 < 0 && p->unk4c <= 8 && gUnknown_030059C0[p->unk64] == 0)
    {
        SetObjAffine(0,
                     Div(gSinLut[0x40] * 16, Interpolate(0, 0x100, 0x10, p->unk4c, 8) != 0 ? Interpolate(0, 0x100, 0x10, p->unk4c, 8) : 2),
                     Div(-gSinLut[0] * 16, 0x100),
                     Div(gSinLut[0] * 16, Interpolate(0, 0x100, 0x10, p->unk4c, 8) != 0 ? Interpolate(0, 0x100, 0x10, p->unk4c, 8) : 2),
                     Div(gSinLut[0x40] * 16, 0x100));
        sub_08043B60(0x2a, ((i + 1) * 0x10 + 0x68) | 0x100, ((i + 1) * 0xc + 0x3c0) | 0x9000, 0);
    }

    if (p->unk66 != 0 && p->unk4c == 8)
        p->unk66 = 0;

    if (gUnknown_030059C0[p->unk64] != 0)
    {
        for (i = 0; i < p->unk64; i++)
            DrawOamObject(gUnknown_03005958[gUnknown_0300599C[i]] + 0x3e, 0x14, i * 0x10 + 0x68, 0, 2);

        if (i < sub_0803BD14())
            DrawOamObject(gUnknown_03005958[p->unk58] + 0x3e, 0x14, i * 0x10 + 0x68, 0, 2);
    }
    else
    {
        for (i = 0; i < sub_0803BD14(); i++)
            DrawOamObject(gUnknown_03005958[i] + 0x3e, 0x14, i * 0x10 + 0x68, 0, 2);
    }

    if (p->unk40 > 0)
    {
        if (p->unk4c <= 7 && p->unk6a != 0)
        {
            SetObjAffine(1,
                         Div(gSinLut[0x40] * 16, Interpolate(1, 0x200, 0x100, p->unk4c, 7) != 0 ? Interpolate(1, 0x200, 0x100, p->unk4c, 7) : 2),
                         Div(-gSinLut[0] * 16, Interpolate(1, 0x200, 0x100, p->unk4c, 7) != 0 ? Interpolate(1, 0x200, 0x100, p->unk4c, 7) : 2),
                         Div(gSinLut[0] * 16, Interpolate(1, 0x200, 0x100, p->unk4c, 7) != 0 ? Interpolate(1, 0x200, 0x100, p->unk4c, 7) : 2),
                         Div(gSinLut[0x40] * 16, Interpolate(1, 0x200, 0x100, p->unk4c, 7) != 0 ? Interpolate(1, 0x200, 0x100, p->unk4c, 7) : 2));
            DrawOamObject(0x4f, 0x24a, 0x360, 0, 0);
        }
        else
        {
            DrawOamObject(0x4f, 0x5a, 0x68, 0, 0);
        }
    }
    else
    {
        DrawOamObject(0x54, 0x5c, 0x68, 0, 0);
        c = sub_0803BD14();
        d = p->unk64 - 0x55;
        DrawOamObject(c - d, 0x65, 0x70, 0, 0);
        DrawOamObject(0x5f, 0x6d, 0x70, 0, 0);
    }
}
asm(".global sub_0807DA98\n.thumb_set sub_0807DA98, CoSelect_DrawSprites\n");
