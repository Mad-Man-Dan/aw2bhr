#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804F0D8.
 * sub_0804F0D8 @ 0x0804F0D8
 */

void ApcFigure_Loop(void)
{
    struct Unk02029A10 *entry;
    u16 w;
    u16 *p;
    u16 *row;
    u16 c, e;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    w = StepFigureHitFlash2(c, e, gUnknown_03001FBC);
    p = *(u16 **)(c * sizeof(u16 *) + (u8 *)gUnknown_084C3F78);
    entry = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                   + c * sizeof(struct Unk02029A10Group)
                                   + (u8 *)gUnknown_02029A10);
    entry->x += gUnknown_08553B28[c][w];
    entry->y -= gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]];
    SetSlotSpritePosition(gUnknown_03001FBC, entry->x, entry->y - *p);
}
asm(".global sub_0804F0D8\n.thumb_set sub_0804F0D8, ApcFigure_Loop\n");
