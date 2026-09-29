#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051BEC.
 * sub_08051BEC @ 0x08051BEC
 */

/* MATCHED. The near-twin of DeathHandler_Air -- same install sequence, differing
 * in the sub_08015410 blob (gUnknown_085536A4), the gUnknown_08553B10 table,
 * an extra SetSlotSpriteFlicker call, the 0x180 rather than 0x100 handed to
 * SetSlotSpriteScaleY, and the tail. See DeathHandler_Air for the `f`, `c`/`d` and
 * pointer-local notes; all three carry over except that this one needs no
 * `row` local, because its gUnknown_085D6A48 read is column 0 and has no
 * constant to misplace.
 *
 * The tail is an indirect call through gUnknown_08552FB8 column 1, indexed
 * `[gUnknown_085D6A48[gUnknown_03004580[a][1]][0]][gUnknown_03004580[a][2]]`
 * -- the same `[w][q][c]` shape CruiserFigure_Init/BattleshipFigure_Init use on column 2.
 * `bl _call_via_r3` is ordinary compiler output for a three-argument indirect
 * THUMB call, not a veneer.
 *
 * `entries[b].unk00 = entries[b].unk01;` and NOT `= 1`, which is the one place
 * this function and DeathHandler_Air disagree on an identical-looking statement.
 * Both are semantically the same under the `unk01 == 1` guard, and each
 * matches only its own function -- probed both ways in both functions, and the
 * literal costs a surplus `movs r0,#1` here while the member read costs an
 * `adds r2,r3,#0` there. The cause is visible in the listing: here the second
 * `unk01` test is a fresh `ldrb` (the indirect call in between clobbers
 * memory), so cse's "r0 holds 1 on this edge" equivalence is still free to
 * supply the store; in DeathHandler_Air the two tests share one load and the
 * register is committed. Do not "clean this up" to make the siblings look
 * alike -- it is a measured difference, not a transcription choice. */
void DeathHandler_Tank(u16 a, u16 b)
{
    u16 c;
    u16 d;
    u16 e;
    int f;

    c = a;
    d = b;

    gUnknown_02028E5C[a][0] = 1;
    *gUnknown_084C3F78[a] = 0;
    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    f = gUnknown_08552178[a][b] * 7 + 6;

    gUnknown_02029808[a].unk24[b] = sub_08015410(gUnknown_085536A4, 1,
        gUnknown_02029808[a].unk44[gUnknown_02029808[a].unk2e],
        gUnknown_02029808[a].unk58[gUnknown_02029808[a].unk2e],
        f);

    SetSlotSpriteFlicker(gUnknown_02029808[a].unk24[b], 1);
    PlayFigureDestroySfx(a, 0x10);

    e = gUnknown_08553B10[a];

    EnableSlotSpriteAffine(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteDoubleSize(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
    SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x180);

    if (gUnknown_02029A10[a].entries[b].unk01 == 1)
    {
        gUnknown_02029A10[a].entries[b].unk00 = gUnknown_02029A10[a].entries[b].unk01;
        ((void (*)(u16, u16, int))gUnknown_08552FB8
            [gUnknown_085D6A48[gUnknown_03004580[a][1]][0]]
            [gUnknown_03004580[a][2]][1])(a, b, 0);
    }

    if (gUnknown_02029C14[a][d] != 1 && gUnknown_02029A10[a].entries[b].unk01 == 0)
        StartFigureFall(c, d);
}
asm(".global sub_08051BEC\n.thumb_set sub_08051BEC, DeathHandler_Tank\n");
