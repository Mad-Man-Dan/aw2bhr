#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805F7B8.
 * sub_0805F7B8 @ 0x0805F7B8
 */

/* AiFallbackMove @ 0x0805F7B8, 348 bytes. MATCHED.
 *
 * The "no order given -- pick a target yourself" fallback of the battle-cursor
 * block: it scans every cell, scores it, and issues the best one as an ordinary
 * AiPublishAction move command before longjmping out through sub_08071910. Twenty
 * functions tail-call it, which is why it reads as a bail-out everywhere else
 * in the block.
 *
 * gUnknown_0816DAB0 is NOT a global: the ROM word there holds 0x030040D8, so it
 * is agbcc's own -fforce-addr constant for gUnknown_030040D8. The honest
 * spelling reproduces it and the promotion must carry the rodata word --
 * data/promoted.json needs "rodata": ["0x0816DAB0"] for this function.
 *
 * Three spellings in the loop body are load-bearing and each was isolated with
 * compile_probe (wave 53, W53-E); all three are written up in
 * docs/agbcc-codegen.md.
 *
 *   - `cell` is BOUND and `*(s8 *)cell` is then read TWICE, once for the `< 0`
 *     test and once for `v`. The binding splits the address and the value into
 *     separate pseudos, and the second read is what CSE turns into the ROM's
 *     `adds r3,r0,#0` copy. A single `v = (s8)gUnknown_03003340[y][x];` gives
 *     one pseudo and loses the copy. (Binding is NOT what makes the load
 *     `ldrsb` -- the unbound spelling gets `ldrsb` here too. See the wave-53
 *     correction in the `ldrsb` chapter.)
 *
 *   - the score test is written INVERTED, as `unk00 == 0 || unk1a != 0x10`,
 *     because the ROM emits the LONG arm first and branches forward past the
 *     literal pool to the short one. agbcc lays the `then` arm out inline, so
 *     the natural way round puts the short arm first and the pool 0x34 bytes
 *     early -- with every instruction present and correct.
 *
 *   - `X * 10 - (n = v - 0x14)` rather than `X * 10 - (v - 0x14)`. fold's
 *     associate block rewrites `A - (B - K)` into `(A + K) - B`, and
 *     cross-jumping then merges the two arms' `subs r4,r0,r3` tails, costing 6
 *     bytes. The embedded assignment makes the right operand a MODIFY_EXPR that
 *     split_tree cannot take apart, and it keeps the evaluation order (minuend
 *     first) that a preceding `n = v - 0x14;` statement would invert. Both
 *     properties are needed and only this spelling gives both.
 *
 * sub_08071910 is the NORETURN THUMB->ARM veneer for LONGJMP, which is why
 * there is no epilogue.
 */

void AiFallbackMove(void)
{
    int x;
    int y;
    int v;
    int n;
    int score;
    int bestX;
    int bestY;
    int best;
    u8 *cell;

    bestX = -1;
    bestY = 0;
    best = 0;

    if (!AiIsSettleCellOk(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03))
    {
        GenerateUnitMovementMap(gUnknown_030040D8);

        for (y = 0; y < gMap->height; y++)
        {
            for (x = 0; x < gMap->width; x++)
            {
                cell = gUnknown_03003340[y] + x;
                if (*(s8 *)cell < 0)
                    continue;
                v = *(s8 *)cell;
                if (gUnknown_030040D8->unk00 == 0
                 || gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != 0x10)
                    score = gUnknown_085D584C[gMap->terrain[gMap->rowOffset[y] + x] & 0x1f].unk00 * 10 - (n = v - 0x14);
                else
                    score = 0x14 - v;
                if (score <= best)
                    continue;
                if (!AiIsSettleCellOk(x, y))
                    continue;
                bestX = x;
                bestY = y;
                best = score;
            }
        }

        if (bestX != -1)
            AiPublishAction(bestX, bestY, 2, 0, 0);
    }

    if (GetMapListEntry(gPlaySt.mapID)->unk28 & 1)
        sub_0805F6D4();
    sub_08071910(gUnknown_03004680, 1);
}
asm(".global sub_0805F7B8\n.thumb_set sub_0805F7B8, AiFallbackMove\n");
