#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080627F4.
 * AiAccumulateInfluence @ 0x080627F4
 */

/* Accumulates each army's unit strength into the gUnknown_0202DAD8 grid.
 *
 * The residual was a pure stack-slot swap between two compiler temps, and two
 * declarations decide it, BOTH needed: `int u;` declared before `int t;`, and
 * `c` declared `s16`. Either one alone is 6 or 8 bytes off. Found by the
 * wave-90 permuter with --stack-diffs on (8,473 iterations, first run); every
 * earlier permuter run scored this residual as a tie. */

void AiAccumulateInfluence(u8 a1)
{
    int u;
    int t;
    int m;
    struct Unit *e;
    int bit;
    s16 c;
    int x;
    int y;
    u32 v;

    for (t = 0; t <= 3; t++) {
        bit = (gPlayers[gUnknown_030033EC].unk2c >> t) & 1;
        if (a1 == 1 && bit == 1)
            continue;
        if (a1 == 0 && bit == 0)
            continue;
        for (u = t * 0x40; u < t * 0x40 + 0x40; u++) {
            e = &gUnknown_08499594[u];
            if (e->type == 0)
                continue;
            for (m = 1; m <= 0x18; m++) {
                switch (gUnknown_085D5ABC[e->type].deployLocation - 1) {
                case 0:
                    c = 0;
                    break;
                case 1:
                    c = 1;
                    break;
                case 3:
                    c = 2;
                    break;
                case 15:
                    c = 3;
                    break;
                case 31:
                    c = 4;
                    break;
                default:
                    continue;
                }
                v = e->hp
                    * GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                                   gPlayers[gUnknown_030033EC].coMode,
                                   m, e->type, 0);
                x = e->x >> 2;
                y = e->y >> 2;
                if (gUnknown_085D5ABC[e->type].minRange == 1) {
                    gUnknown_0202DAD8[y][x].unk00[bit * 5 + c] += v;
                    if (y > 0)
                        gUnknown_0202DAD8[y - 1][x].unk00[bit * 5 + c] += v >> 1;
                    if (y <= 8)
                        gUnknown_0202DAD8[y + 1][x].unk00[bit * 5 + c] += v >> 1;
                    if (x > 0)
                        gUnknown_0202DAD8[y][x - 1].unk00[bit * 5 + c] += v >> 1;
                    if (x <= 0xa)
                        gUnknown_0202DAD8[y][x + 1].unk00[bit * 5 + c] += v >> 1;
                } else {
                    gUnknown_0202DAD8[y][x].unk00[bit * 5 + c] += v >> 2;
                    if (y > 0)
                        gUnknown_0202DAD8[y - 1][x].unk00[bit * 5 + c] += v;
                    if (y <= 8)
                        gUnknown_0202DAD8[y + 1][x].unk00[bit * 5 + c] += v;
                    if (x > 0)
                        gUnknown_0202DAD8[y][x - 1].unk00[bit * 5 + c] += v;
                    if (x <= 0xa)
                        gUnknown_0202DAD8[y][x + 1].unk00[bit * 5 + c] += v;
                }
            }
        }
    }
}
asm(".global sub_080627F4\n.thumb_set sub_080627F4, AiAccumulateInfluence\n");
