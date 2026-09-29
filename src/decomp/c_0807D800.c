#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807D800.
 * IsCoSelectionUnique @ 0x0807D800, CoSelect_ReloadCarouselFaces @ 0x0807D860
 */

#include "proc.h"
#include "hardware.h"
struct Unk807D800
{
    /* 0x00 */ u8 filler_00[0x52];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[4];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[4];
    /* 0x64 */ s16 unk64;
};
struct Unk807D860
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[4];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[4];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

int IsCoSelectionUnique(struct Unk807D800 *p)
{
    int i;

    for (i = 0; i < p->unk64; i++)
    {
        if (gUnknown_030058D4[i] == gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c])
            return 0;
    }

    return 1;
}

asm(".global sub_0807D800\n.thumb_set sub_0807D800, IsCoSelectionUnique\n");

void CoSelect_ReloadCarouselFaces(struct Unk807D860 *p)
{
    if (p->unk4c == 9)
        LoadCoFullBodyAndPalette(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], 0x40, 1);

    if (p->unk4c == 0x10)
    {
        LoadCoFace(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c],
                     (void *)0x06013000, 0x12);
        LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c],
                     (void *)0x06013480, 0x13);
        LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c],
                     (void *)0x06013900, 0x14);
    }
}
asm(".global sub_0807D860\n.thumb_set sub_0807D860, CoSelect_ReloadCarouselFaces\n");
