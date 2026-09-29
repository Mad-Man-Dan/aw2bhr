#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E9F8.
 * sub_0803E9F8 @ 0x0803E9F8
 */

struct Unk0803E9F8
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02_0:6;
               u16 unk02_6:4;
               u16 unk02_a:4;
               u16 unk02_e:2;
    /* 0x04 */ u8 unk04;
};

int MarkInventionFireArea(struct Unk0803E9F8 *p, u8 *a2, u8 a3, u8 a4)
{
    int j;
    int k;

    if (p->unk04 == 0)
        return 0;
    SetWorkingMapPlane(a2);
    FillMovementMap(a3);
    switch (p->unk02_6)
    {
    case 1:
        MapSetCross(p->unk00, p->unk01, a4);
        return 1;
    case 3:
        sub_0801FAC4((u16)(p->unk00 + (int)GetCannonFireCellOffsetX(p->unk02_6, p->unk02_e)),
                     (u16)(p->unk01 + (int)GetCannonFireCellOffsetY(p->unk02_6, p->unk02_e)),
                     p->unk02_e, 0xa, a4);
        return 1;
    case 4:
        sub_0801FAC4((u16)(p->unk00 + (int)gUnknown_0849F698[p->unk02_e].unk00),
                     (u16)((k = p->unk01, j = p->unk02_e * 2 + 1, k)
                           + (int)((void **)gUnknown_0849F698)[j]),
                     p->unk02_e, 4, a4);
        return 1;
    case 5:
        MapSetBarToBottom(p->unk00, p->unk01 + 3, a4);
        return 1;
    }
    return 0;
}
asm(".global sub_0803E9F8\n.thumb_set sub_0803E9F8, MarkInventionFireArea\n");
