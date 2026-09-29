#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804F8BC.
 * sub_0804F8BC @ 0x0804F8BC
 */

void FlyerFigure_Loop(void)
{
    struct Unk02029A10 *entry;
    u16 c, e, t, w;
    u16 *p;
    u16 *row;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    t = sub_080156A0(gUnknown_03001FBC);
    StepFigureSlide(c, e);
    if (t == 2
        && (u32)gUnknown_02029BA8[c].unk14
           != gUnknown_0200E438[gUnknown_03001470[gUnknown_03001FBC].unk26].unk04)
    {
        struct Unk02029A10 *q;

        q = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                   + c * sizeof(struct Unk02029A10Group)
                                   + (u8 *)gUnknown_02029A10);
        if (q->unk18 != -1 && IsSlotScriptActiveAt(q->unk18))
            sub_080156E8(q->unk18, gUnknown_02029BA8[c].unk04);
    }
    w = StepFigureHitFlash(c, e, gUnknown_03001FBC);
    p = *(u16 **)(c * sizeof(u16 *) + (u8 *)gUnknown_084C3F78);
    sub_080156C4(gUnknown_03001FBC);
    entry = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                   + c * sizeof(struct Unk02029A10Group)
                                   + (u8 *)gUnknown_02029A10);
    entry->x += gUnknown_08553B28[c][w];
    entry->y -= gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]];
    StepFigureBob(c, e, gUnknown_03001FBC);
    SetSlotSpritePosition(gUnknown_03001FBC, entry->x, entry->y - *p);
}
asm(".global sub_0804F8BC\n.thumb_set sub_0804F8BC, FlyerFigure_Loop\n");
