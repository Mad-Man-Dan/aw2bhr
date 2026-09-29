#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805F2B4.
 * AiProtectHq @ 0x0805F2B4
 */

/* AiMoveToHighInfluence's twin: same double scan over the map, same 4-byte (u16, u16)
 * buffer seeded with the 0x270F sentinel and handed to AiAdvanceToward, but the
 * ranking key is the +0x2D5A plane rather than the gUnknown_0202DAD8 influence
 * record, and it is gated on IsPlayerAliveAndActive plus two SetWorkingMapPlane plane rebuilds.
 * Read work/AiMoveToHighInfluence/AiMoveToHighInfluence.c first -- every lever is documented
 * there and all of them transferred.
 *
 * WAVE 66 PARK: 96.6%, size exact (536), 18 bytes different. One uninterrupted
 * 300-second four-thread permuter run improved the old 91.8% draft through two
 * legal lifetime anchors: `new_var2` for the 0x10 comparison and `new_var` for
 * the selected cost. No byte-exact candidate was found. The remaining actual
 * byte residual is the pre-loop t/table allocation block plus the transposed
 * shift in the packed store. The gUnknown_085D583C+0x10 versus
 * gUnknown_085D584C relocation is linker-equivalent, not a byte residual.
 *
 * THE TERNARY IS LOAD-BEARING and it was worth 47 points (44.2% -> 91.8%).
 * Written the obvious way --
 *     t = gUnknown_030040D8->unk00;
 *     if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1a == 0x20) t = 0x11;
 * -- agbcc coalesces the copy (it loads straight into t's register, one
 * instruction short of the ROM's `ldrb r1` + `adds r7, r1, #0`) AND expands the
 * gUnknown_030040D8 reference first, which puts that pool word ahead of
 * gUnknown_085D5ABC and shifts every later `ldr rN, [pc, #..]` displacement in
 * the function. One fact, not two.
 *   WAVE 66 CORRECTION: that last claimed readout was wrong for this function.
 * The ternary emits `movs r7,#0x11 / cmp / beq / adds r7,r3,#0`; the ROM emits
 * `adds r7,r1,#0 / cmp / bne / movs r7,#0x11`. Binding t first and indexing the
 * table through t also misses: it coalesces the initial load into r7, removes
 * the copy, and shifts the pool. Combining that spelling with the matched
 * twin's xp/yp packed-store temporaries regressed to 46.5%, so the independently
 * verified 96.6% fixpoint above was restored.
 *
 * WAVE 70 (W70-C): MATCHED configured, 536/536. The twin's xp/yp temporaries
 * do transfer once applied to the readable 96.6% active draft. The pre-loop
 * allocation closes by binding the table, fixing the selector local to r1,
 * and binding the table tag before assigning t: this makes the selector and t
 * distinct allocnos and emits the ROM's `ldrb r1`, table lookup, `adds r7,r1`,
 * compare, and conditional r7=0x11 in that exact order. The fixed-r1 local is
 * load-bearing; ordinary u8/int selector locals coalesce straight into r7.
 *
 * As in the twin: `p` is assigned AFTER the loop nest (before it, or inside
 * either loop, both misplace the &v binding), and gUnknown_0816DAA8 is NOT an
 * object -- the ROM word holds 0x030040D8, so it is agbcc's own -fforce-addr
 * constant for gUnknown_030040D8 and the promotion carries
 * "rodata": ["0x0816DAA8"].
 *
 * The plane rebuild calls pass gMap's danger and move members directly; the
 * arrays decay to the `u8 *` parameter type without raw map-pointer arithmetic. */
void AiProtectHq(void)
{
    union Unk802C57CBuf v;
    int new_var2;
    u8 cost;
    int best;
    u8 new_var;
    int second;
    u32 xp;
    u32 yp;
    u16 *p;
    int t;
    struct UnitType *table;
    register u8 type asm("r1");
    u8 tag;
    int a;
    int b;
    int x;
    int y;
    int score;

    best = 0;
    second = 0xff;

    if (!IsPlayerAliveAndActive(gUnknown_030033EC))
    {
        AiFallbackMove();
        return;
    }

    table = gUnknown_085D5ABC;
    type = gUnknown_030040D8->unk00;
    tag = table[type].deployLocation;
    t = type;
    if (tag == 0x20)
        t = 0x11;

    a = gPlayers[gUnknown_030033EC].hqX & 0x7f;
    b = gPlayers[gUnknown_030033EC].hqY & 0x7f;

    SetWorkingMapPlane(gMap->danger);
    gUnknown_030013EC(a, b, t, 5, best);
    AiGetReachBudget(&cost);
    SetWorkingMapPlane(gMap->move);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, cost, best);
    v.raw = (v.raw & 0xFFFF0000) | 0x270F;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;

            if (gMap->danger[gMap->rowOffset[y] + x] <= 0)
                continue;

            new_var2 = 0x10;
            if (gUnknown_030040D8->unk00 == 0
             || gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != new_var2)
                score = gUnknown_085D583C[
                    gMap->terrain[gMap->rowOffset[y] + x] & 0x1f].defense * 10;
            else
                score = 0;

            if (best <= score
             && second > gMap->danger[gMap->rowOffset[y] + x])
            {
                best = score;
                new_var = (u8)gMap->danger[gMap->rowOffset[y] + x];
                second = new_var;
                xp = (u32)x << 16;
                yp = (u32)y << 16;
                v.raw = (xp >> 16) | yp;
            }
        }
    }

    p = (u16 *)&v;

    if (p[0] == 0x270F)
        AiFallbackMove();
    AiAdvanceToward(p);
    AiFallbackMove();
}

asm(".global sub_0805F2B4\n.thumb_set sub_0805F2B4, AiProtectHq\n");
