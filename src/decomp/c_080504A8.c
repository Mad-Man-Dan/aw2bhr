#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080504A8.
 * sub_080504A8 @ 0x080504A8
 */

void PlayFigureDestroySfx(u16 a1, u16 a2)
{
    if (gUnknown_085644A0[gUnknown_02029808[a1].unk30[gUnknown_02029808[a1].unk2e]]
        == 0xFF)
    {
        m4aSongNumStop(0x3B);
        m4aSongNumStop(0x3C);
        PlayMusicOrSfx(a2);
    }
    else
    {
        m4aSongNumStop(0x3B);
        m4aSongNumStop(0x3C);
        PlayMusicOrSfx(
            gUnknown_085644A0[gUnknown_02029808[a1].unk30[gUnknown_02029808[a1].unk2e]]);
    }
}
asm(".global sub_080504A8\n.thumb_set sub_080504A8, PlayFigureDestroySfx\n");
