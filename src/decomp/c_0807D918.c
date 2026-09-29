#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807D918.
 * sub_0807D918 @ 0x0807D918
 */

/* Six copies of one display-list push, selected by an s16 state at +0x4c and
 * by the sign of the s32 at +0x60. The parameter is modelled with filler
 * rather than PROC_HEADER: nothing here touches a proc field or calls a
 * Proc_* routine, so there is no evidence it is a Proc. */
struct Unk7D918
{
    /* 00 */ u8 filler_00[0x4c];
    /* 4C */ s16 unk4c;
    /* 4E */ u8 filler_4e[0x04];
    /* 52 */ u16 unk52;
    /* 54 */ u8 filler_54[0x04];
    /* 58 */ int unk58;
    /* 5C */ int unk5c;
    /* 60 */ int unk60;
};

void CoSelect_ReloadFacesOnGroupSwitch(struct Unk7D918 *p)
{
    if (p->unk4c == 13)
        LoadCoFullBodyAndPalette(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], 0x40, 1);

    if (p->unk60 < 0)
    {
        if (p->unk4c == 13)
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013000, 0x12);

        if (p->unk4c == 14)
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013480, 0x13);

        if (p->unk4c == 15)
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013900, 0x14);
    }
    else if (p->unk60 > 0)
    {
        if (p->unk4c == 13)
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013000, 0x12);

        if (p->unk4c == 9)
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013480, 0x13);

        if (p->unk4c == 6)
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013900, 0x14);
    }
}
asm(".global sub_0807D918\n.thumb_set sub_0807D918, CoSelect_ReloadFacesOnGroupSwitch\n");
