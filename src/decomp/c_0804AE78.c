#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AE78.
 * sub_0804AE78 @ 0x0804AE78
 */

void NameEntry_SlideIn_Step(void)
{
    u16 i;
    u16 j;
    u16 v;
    struct Unk030044E0 *p;
    int lim;
    int k;

    gUnknown_03001FF8 += 6;
    gUnknown_030030A0 = gUnknown_030030A0 + 6;
    lim = 0xf0;
    v = gUnknown_030030A0;

    if (v > 0x1d8)
    {
        gUnknown_03001FF8 = 0x1d9;
        gUnknown_030030A0 = 0x1d8;
    }

    for (i = 0; i < 0x14; i++)
    {
        k = ((gUnknown_030030A0 - 0x138) >> 3) + 3;
        k &= 0x1f;
        v = k;

        for (j = v; j < (v + 3); j++)
            gBG2TilemapBuffer[(i * 0x20) + j] = gBG1TilemapBuffer[(i * 0x20) + j];
    }

    BG_EnableSyncBG2();

    p = gUnknown_030044E0;
    p->unk2a = (gUnknown_030044E0->unk2a > 0xef) ? (0xf0) : (gUnknown_030044E0->unk2a + 3);

    if ((gUnknown_030030A0 == 0x1d8) && (gUnknown_030044E0->unk2a == lim))
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_0804AE78\n.thumb_set sub_0804AE78, NameEntry_SlideIn_Step\n");
