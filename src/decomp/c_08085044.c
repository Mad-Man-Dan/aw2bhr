#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085044.
 * sub_08085044 @ 0x08085044
 */

/* The `(t = p->unk2c - 4)` inline assignment is the wave-54/W54-B fold barrier:
 * written honestly as `(p->unk66 - 1) * 0x18 + p->unk2c - 4`, fold reassociates
 * the -4 onto the multiply and emits `subs r1, #4` before the `ldr` of unk2c.
 *
 * The two `==` tests branching AWAY with the third arm inline is a `switch`,
 * not an if/else-if chain: an if/else-if emits `bne` around an inline first
 * arm (measured -- it cost 4 bytes and moved the literal pool).
 *
 * gUnknown_08239F84 is an ARRAY (SYMBOL_REF), so `&gUnknown_08239F84[i]` loads
 * the base BEFORE scaling the index; the ROM scales first, which needs the
 * integer-add spelling with the base on the RIGHT (wave 54, W54-B).
 */

struct Unk8085044
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4e);
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x58);
    /* 58 */ int unk58;
    /* 5c */ STRUCT_PAD(0x5c, 0x66);
    /* 66 */ s16 unk66;
};

void CoInfoScreen_DrawCursorAndArrows(struct Unk8085044 *p)
{
    int t;

    if (DivRem(p->unk58, 0x20) <= 0xF)
    {
        p->unk2c = Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10);
        p->unk30 = Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10);
    }
    else
    {
        p->unk2c = Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10);
        p->unk30 = Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10);
    }

    DrawOamObject(0x50, ((p->unk66 - 1) * 0x18 + (t = p->unk2c - 4)) & 0x1FF,
                 (0x14 - p->unk30) & 0xFF, 0, 1);

    switch (gUnknown_03005940)
    {
    case 0:
        DrawOamObject(0x44, 0x64, 0x98, 0, 1);
        break;
    case 4:
        DrawOamObject(0x43, 0xDC, 0x20, 0, 1);
        break;
    default:
        DrawOamObject(0x43, 0x64, 0x20, 0, 1);
        DrawOamObject(0x44, 0x64, 0x98, 0, 1);
        break;
    }

    if (p->unk4e >= 0)
        ApplyPaletteExt((u16 *)(DivRem(Div(p->unk58, 4), 0x10) * 2 + (int)gUnknown_08239F84),
                        0x298, 2);

    p->unk58++;
}
asm(".global sub_08085044\n.thumb_set sub_08085044, CoInfoScreen_DrawCursorAndArrows\n");
