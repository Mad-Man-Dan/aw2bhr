#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052AF4.
 * sub_08052AF4 @ 0x08052AF4
 */

/* MATCHED. Byte-for-byte the same function as SplashEffect_Loop -- same instruction
 * stream, same pool words in the same order, verified by diffing the two
 * listings with the local labels stripped. One C body installed from two
 * different animation descriptors (gUnknown_085536EC and gUnknown_08553704),
 * so the two copies need distinct addresses. Read SplashEffect_Loop's comment; there
 * is nothing here that is not there, including the `c`/`d` refutation. */
void BattleAnimExplosion_Loop(void)
{
    u16 a;
    u16 b;
    u16 e;

    gUnknown_03001470[gUnknown_03001FBC].unk28++;

    if (gUnknown_03001470[gUnknown_03001FBC].unk28 == 8)
    {
        a = gUnknown_03001470[gUnknown_03001FBC].unk30;
        b = gUnknown_03001470[gUnknown_03001FBC].unk34;

        e = gUnknown_08553B10[a];

        if (gUnknown_02029A10[a].entries[b].unk1c == 1)
            DeathHandler_Fall(a, b, 0);

        gUnknown_02028E5C[a][0] = 1;
        *gUnknown_084C3F78[a] = 0;

        SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
        SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x180);
    }
}
asm(".global sub_08052AF4\n.thumb_set sub_08052AF4, BattleAnimExplosion_Loop\n");
