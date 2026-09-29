#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805131C.
 * sub_0805131C @ 0x0805131C
 */

void TankHitEffect_Loop(void)
{
    u16 a;
    u16 b;
    const u16 *r;

    a = gUnknown_03001470[gUnknown_03001FBC].unk30;
    b = gUnknown_03001470[gUnknown_03001FBC].unk34;
    gUnknown_03001470[gUnknown_03001FBC].unk2c++;

    if (gUnknown_03004580[a ^ 1][2] == 2
        && gUnknown_03001470[gUnknown_03001FBC].unk2c > 3)
    {
        gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
        gUnknown_020298E0[gUnknown_0300453C].unk8d++;
    }

    if (gUnknown_02029A10[a].entries[b].unk00 == 0
        && (r = gUnknown_085D6A48[gUnknown_03004580[a][1]])[1] == 1)
        EndSlotScriptAt(gUnknown_03001FBC);

    RaiseHitDoneStageFlag(a, b, gUnknown_03001FBC);
}
asm(".global sub_0805131C\n.thumb_set sub_0805131C, TankHitEffect_Loop\n");
