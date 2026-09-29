#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C098.
 * sub_0804C098 @ 0x0804C098
 */

struct Unk85D6A48Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 filler_02[0x10];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[4];
};

void CopyFigureSheetToVram(u16 a)
{
    struct Unk85D6A48Row *rows;
    u16 off;

    rows = (struct Unk85D6A48Row *)gUnknown_085D6A48;

    if (rows[gUnknown_03004580[a][1]].unk12 == 0)
    {
        off = a * 0x2000;
        CpuCopyAuto(gUnknown_02029BA8[a].unk18[0], (void *)(0x06010000 + off),
                     0x2000);
    }
}
asm(".global sub_0804C098\n.thumb_set sub_0804C098, CopyFigureSheetToVram\n");
