#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08088CDC.
 * sub_08088CDC @ 0x08088CDC, sub_08088DA4 @ 0x08088DA4
 */

struct Unk08088CDC
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[4];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[4];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};
struct Unk08088DA4
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[4];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[4];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void CoDesignEditor_LoadCoGraphicsMidSlide(struct Unk08088CDC *p)
{
    int v0;
    int v1;
    int v2;
    s16 *m;

    v0 = gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c];
    v1 = gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c];
    v2 = gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c];

    m = &p->unk4c;

    if (*m == 9)
        LoadCoFullBodyPart0(v0, 0x40);

    if (*m == 0xa)
    {
        LoadCoFullBodyPart1(v0, 0x40);
        LoadCoPalette(v0, 0x11);
    }

    if (*m == 0x10)
    {
        LoadCoFace(v0, (void *)0x06013000, 0x12);
        LoadCoFace(v1, (void *)0x06013480, 0x13);
        LoadCoFace(v2, (void *)0x06013900, 0x14);
    }
}
asm(".global sub_08088CDC\n.thumb_set sub_08088CDC, CoDesignEditor_LoadCoGraphicsMidSlide\n");

void CoDesignEditor_LoadCoGraphicsMidGroupSlide(struct Unk08088DA4 *p)
{
    int v0;
    int v1;
    int v2;
    s16 *m;

    v0 = gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c];
    v1 = gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c];
    v2 = gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c];

    m = &p->unk4c;

    if (*m == 0xd)
        LoadCoFullBodyPart0(v0, 0x40);

    if (*m == 0xe)
    {
        LoadCoFullBodyPart1(v0, 0x40);
        LoadCoPalette(v0, 0x11);
    }

    if (p->unk60 < 0)
    {
        if (*m == 0xd)
            LoadCoFace(v0, (void *)0x06013000, 0x12);

        if (*m == 0xe)
            LoadCoFace(v1, (void *)0x06013480, 0x13);

        if (*m == 0xf)
            LoadCoFace(v2, (void *)0x06013900, 0x14);
    }
    else if (p->unk60 > 0)
    {
        if (*m == 0xd)
            LoadCoFace(v0, (void *)0x06013000, 0x12);

        if (*m == 9)
            LoadCoFace(v1, (void *)0x06013480, 0x13);

        if (*m == 6)
            LoadCoFace(v2, (void *)0x06013900, 0x14);
    }
}
asm(".global sub_08088DA4\n.thumb_set sub_08088DA4, CoDesignEditor_LoadCoGraphicsMidGroupSlide\n");
