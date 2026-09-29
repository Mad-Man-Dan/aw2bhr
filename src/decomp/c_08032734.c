#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032734.
 * sub_08032734 @ 0x08032734, sub_08032788 @ 0x08032788, sub_080327FC @ 0x080327FC, sub_08032850 @ 0x08032850
 */

#include "proc.h"

void LinkMapPickSlideIn_Init(void)
{
    gUnknown_0849B060->unk0a = 15;

    if (gUnknown_0849B060->unk10 == 1)
        gUnknown_0849B060->unk0e = -gUnknown_0849B650[gUnknown_0849B060->unk0a];

    if (gUnknown_0849B060->unk10 == 2)
        gUnknown_0849B060->unk0e = gUnknown_0849B650[gUnknown_0849B060->unk0a];

    gUnknown_03002F18 = 0xFFD4;
    gUnknown_03002B34 = gUnknown_0849B060->unk0e - 0x60;
}
asm(".global sub_08032734\n.thumb_set sub_08032734, LinkMapPickSlideIn_Init\n");

void LinkMapPickSlideIn_Loop(ProcPtr proc)
{
    gUnknown_0849B060->unk0a--;

    if (gUnknown_0849B060->unk10 == 1)
        gUnknown_0849B060->unk0e = -gUnknown_0849B650[gUnknown_0849B060->unk0a];

    if (gUnknown_0849B060->unk10 == 2)
        gUnknown_0849B060->unk0e = gUnknown_0849B650[gUnknown_0849B060->unk0a];

    gUnknown_03002F18 = 0xFFD4;
    gUnknown_03002B34 = gUnknown_0849B060->unk0e - 0x60;

    if (gUnknown_0849B060->unk0a == 0)
    {
        PlayMusicOrSfx2(0x76);
        Proc_Break(proc);
    }
}
asm(".global sub_08032788\n.thumb_set sub_08032788, LinkMapPickSlideIn_Loop\n");

void LinkMapPickSlideOut_Init(void)
{
    gUnknown_0849B060->unk0a = 0;

    if (gUnknown_0849B060->unk10 == 1)
        gUnknown_0849B060->unk0e = gUnknown_0849B650[gUnknown_0849B060->unk0a];

    if (gUnknown_0849B060->unk10 == 2)
        gUnknown_0849B060->unk0e = -gUnknown_0849B650[gUnknown_0849B060->unk0a];

    gUnknown_03002F18 = 0xFFD4;
    gUnknown_03002B34 = gUnknown_0849B060->unk0e - 0x60;
}
asm(".global sub_080327FC\n.thumb_set sub_080327FC, LinkMapPickSlideOut_Init\n");

void LinkMapPickSlideOut_Loop(ProcPtr proc)
{
    gUnknown_0849B060->unk0a++;

    if (gUnknown_0849B060->unk10 == 1)
        gUnknown_0849B060->unk0e = gUnknown_0849B650[gUnknown_0849B060->unk0a];

    if (gUnknown_0849B060->unk10 == 2)
        gUnknown_0849B060->unk0e = -gUnknown_0849B650[gUnknown_0849B060->unk0a];

    gUnknown_03002F18 = 0xFFD4;
    gUnknown_03002B34 = gUnknown_0849B060->unk0e - 0x60;

    if (gUnknown_0849B060->unk0a == 0xf)
        Proc_Break(proc);
}
asm(".global sub_08032850\n.thumb_set sub_08032850, LinkMapPickSlideOut_Loop\n");
