#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08055F68.
 * sub_08055F68 @ 0x08055F68
 */

struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u8 filler_00[0x02];
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x02];
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u8 filler_0a[0x0e];
};

u16 GetDeathEffectKind(u16 a)
{
    struct Unk85D6A48Row *tbl;
    struct Unk85D6A48Row *t2;
    struct Unk85D6A48Row *t3;
    u16 add;

    add = 0;
    if (gUnknown_03004580[a][2] == 1
        && (tbl = (struct Unk85D6A48Row *)gUnknown_085D6A48,
            tbl[gUnknown_03004580[a][1]].unk06) != 0)
    {
        if (gUnknown_03004580[a ^ 1][3] == 2
            || tbl[gUnknown_03004580[a ^ 1][1]].unk02 == 2)
            add = 0xc;
        else
            add = 4;
        return add + (t2 = (struct Unk85D6A48Row *)gUnknown_085D6A48,
                      t2)[gUnknown_03004580[a][1]].unk06;
    }
    return add + (t3 = (struct Unk85D6A48Row *)gUnknown_085D6A48,
                  t3)[gUnknown_03004580[a ^ 1][1]].unk08;
}
asm(".global sub_08055F68\n.thumb_set sub_08055F68, GetDeathEffectKind\n");
