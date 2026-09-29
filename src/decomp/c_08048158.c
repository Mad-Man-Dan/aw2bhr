#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048158.
 * sub_08048158 @ 0x08048158
 */

struct Unk08047B98
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 unk1f;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
};

void UnitList_DrawSprites(struct Unk08047B98 *p)
{
    int i;

    if (p->unk22 > 0x1d)
        p->unk22 = 0;

    p->unk22++;

    switch (p->unk1e)
    {
    case 0:
        PutSprite(0, 0x28, 0x28, gUnknown_08615C4E, 0x55ce);
        break;
    case 1:
        PutSprite(0, 0x70, 0x28, gUnknown_0848B6C6, 0x55e2);
        break;
    case 2:
        PutSprite(0, 0x88, 0x28, gUnknown_08615C20, 0x55ea);
        break;
    case 3:
        PutSprite(0, 0xbb, 0x28, gUnknown_08615C12, 0x55fa);
        break;
    }

    if (p->unk20 != 0)
        DrawMapCursorSprite(0x29, 0x38, 0xe);

    if (p->unk21 > 5 && p->unk20 < p->unk21 - 6)
        DrawMapCursorSprite(0x29, 0x97, 0xf);

    i = DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10) * 2;

    ApplyPaletteExt((u16 *)((u8 *)gUnknown_0823E550 + i),
                    0x2a0,
                    (0x10 - DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10)) * 2);

    ApplyPaletteExt(gUnknown_0823E550,
                    (u16)((0x10 - DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10)) * 2 + 0x2a0),
                    DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10) * 2);
}
asm(".global sub_08048158\n.thumb_set sub_08048158, UnitList_DrawSprites\n");
