#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08054598.
 * sub_08054598 @ 0x08054598
 */

void ApplyHitToWholeUnit(u16 a, u16 b)
{
    u16 total;
    u16 d;
    u16 i;

    gUnknown_020298E0[a].unk16++;
    total = 0;
    if (b != gUnknown_08552148[a]) {
        total = gUnknown_02029A10[a].entries[b].unk00 - gUnknown_02029A10[a].entries[b].unk01;
        gUnknown_02029A10[a].entries[b].unk00 -= total;
    }
    if (gUnknown_020298E0[a].unk1a[gUnknown_020298E0[a].unk16 + 1] == 0xff) {
        for (i = 0; i < 5; i++) {
            if (i != gUnknown_08552148[a]) {
                d = gUnknown_02029A10[a].entries[i].unk00 - gUnknown_02029A10[a].entries[i].unk01;
                gUnknown_02029A10[a].entries[i].unk00 -= d;
                total += d;
            } else if (gUnknown_02029A10[a].entries[i].unk01 == 1) {
                d = gUnknown_02029A10[a].entries[i].unk00 - gUnknown_02029A10[a].entries[i].unk01;
                gUnknown_02029A10[a].entries[i].unk00 -= d;
                total += d;
            }
        }
    }
    if (total != 0 && gUnknown_03004580[a][5] != 0)
        gUnknown_03004580[a][5] -= total;
    gUnknown_03004548[a] = 1;
    SpawnHitEffect(a, b);
}
asm(".global sub_08054598\n.thumb_set sub_08054598, ApplyHitToWholeUnit\n");
