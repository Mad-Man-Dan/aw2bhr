#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065DAC.
 * sub_08065DAC @ 0x08065DAC
 */

void MatchSetupCycleCo(int a1, u16 a2, u8 a3)
{
    int v;

    v = gUnknown_08580934->unk1c[a1];

    if (a2 & 0x40)
    {
        int m;

        m = gUnknown_08580934->unk17 - 1;
        v = (v + m) % gUnknown_08580934->unk17;
    }

    if (a2 & 0x80)
    {
        int m;

        m = gUnknown_08580934->unk17 + 1;
        v = (v + m) % gUnknown_08580934->unk17;
    }

    if (v != gUnknown_08580934->unk1c[a1])
    {
        if (a3 != 0)
            PlayMusicOrSfx2(0x64);

        LoadCoFace(gUnknown_08580934->unk18[v],
                     (void *)(0x06010000 + (((a1 * 36 + 400) & 0x3ff) << 5)),
                     a1 + 0x10);

        gUnknown_08580934->unk1c[a1] = v;
    }
}
asm(".global sub_08065DAC\n.thumb_set sub_08065DAC, MatchSetupCycleCo\n");
