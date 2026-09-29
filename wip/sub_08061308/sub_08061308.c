#include "global.h"
#include "map.h"

/* PARKED, wave 56 W56-M; advanced in Wave 72. The retained configured draft
 * is 13.5%, candidate 844 bytes vs 864 (-20), first difference +0x1c.
 * READ THE DIFF, NOT THE SCORE -- this is size-adjacent and almost every
 * instruction is already in the right place. The remaining -20 decomposes:
 *
 *   -12  the ROM keeps the &<pool word 0x0816DAF4> pseudo (the force-addr
 *        constant for gUnknown_08499590) in r8 and pays `mov r2,r8` before the
 *        `ldr` at the head of all five switch cases and once more; this
 *        candidate has it in r7 and reads it directly. The mirror of that swap
 *        is `i * 4`, which the ROM holds in r7 and this candidate in r8.
 *   -8   two extra literal-pool words -- gUnknown_030013EC and
 *        gUnknown_030046C0 appear TWICE in the ROM, once per arm of the
 *        `if (a2 == 4)`, which is the c_0805A95C.c "a non-const global's MEM
 *        does not survive the merge" case; this candidate shares one of each.
 *
 * Wave 72 made case 2 author its accept path before continuing the inner loop.
 * This reproduces the ROM's duplicated `strb; strb; mov; ldr` block exactly
 * and moves 836 -> 844 bytes (-28 -> -20). A Wave-63 300-second permuter run
 * did not close the prior draft. Volatile/cast/bound-local spellings of the
 * first gUnknown_030046C0.unk06 read all keep its address live in r8 and do
 * not reach the remaining allocation. old-agbcc is worse at 828/864 (-36),
 * and old-agbcc-no-force is 804/864 (-60).
 * Wave 78 also tested branch-local function-pointer bindings in both
 * `a2 == 4` arms. They do split the gUnknown_030013EC references, but add too
 * much pressure and produce 872/864 (+8), 38.2%; a volatile function-pointer
 * view compiles identically to this retained 844/864 (-20) draft.
 *
 * SETTLED, read off the assembly and already applied:
 *   - `k = 0` is executed BEFORE the `gUnknown_030045C8 != a1` guard and
 *     before `p = gUnknown_03003F20`, so the loop is `for (; cond; k++)` with
 *     the init hoisted out of the `if`. Writing `for (k = 0; ...)` inside the
 *     `if` puts the `movs/str` after the branch. This moved the first
 *     difference from +0x18 to +0x1c.
 *   - the out cells are c_08059C00.c's `{u8 x; u8 y; s16 v;}` view of
 *     gUnknown_03003F20, and `p` is re-seeded from that global once per outer
 *     iteration, not once per call.
 *   - the terminator is `p->v = 0xffff` written as an int constant (pool
 *     `ldr` + `strh`), not `-1` (which would be `movs #1; rsbs`).
 *   - the 5th argument to the gUnknown_030013EC indirect call is the literal
 *     0; the ROM's `str r4,[sp]` is CSE substituting the register that the
 *     `map->unk12[idx] == 0` test just proved zero -- same phenomenon
 *     c_080591E4.c records. Do not author a variable for it.
 */

struct Unk61308Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

u8 sub_08061308(u8 a1, u8 a2, u16 *a3)
{
    struct Unk61308Cell *p;
    struct Unit *q;
    int k;
    int i;
    int j;
    u16 v;
    struct Map **mp;

    k = 0;
    p = (struct Unk61308Cell *)gUnknown_03003F20;
    if (gUnknown_030045C8 != a1)
    {
        mp = &gMap;
        for (; gUnknown_085766E4[k].unk00 != 0xff; k++)
        {
            if (gUnknown_0857680F[gUnknown_030046C0.unk06] != gUnknown_085766E4[k].unk02)
                continue;
            if (gMap->unit[
                    gMap->rowOffset[gUnknown_085766E4[k].unk01]
                        + gUnknown_085766E4[k].unk00] != 0)
                continue;

            sub_0801F92C(gMap->move);

            if (a2 == 4)
                gUnknown_030013EC(gUnknown_085766E4[k].unk00, gUnknown_085766E4[k].unk01,
                                  0x14, 0x78, 0);
            else
                gUnknown_030013EC(gUnknown_085766E4[k].unk00, gUnknown_085766E4[k].unk01,
                                  gUnknown_030046C0.unk06, 0x78, 0);

            for (i = 0; i < gMap->height; i++)
            {
                for (j = 0; j < gMap->width; j++)
                {
                    if ((s8)gUnknown_03003340[i][j] < 0)
                        continue;
                    switch (a2)
                    {
                    case 0:
                        if (gUnknown_085767D5[(*mp)->terrain[
                                (*mp)->rowOffset[i] + j] & 0x1f] != 1)
                            continue;
                        if (((*mp)->terrain[
                                (*mp)->rowOffset[i] + j] & 0xe0)
                                    == gUnknown_03004084)
                            continue;
                        break;
                    case 1:
                        if ((*mp)->unit[
                                (*mp)->rowOffset[i] + j] == 0)
                            continue;
                        if (((*mp)->unit[
                                (*mp)->rowOffset[i] + j] & 0xc0)
                                    != gUnknown_03003F2C)
                            continue;
                        q = &gUnknown_08499594[(*mp)->unit[
                                (*mp)->rowOffset[i] + j]];
                        if (q->type != 0x14)
                            continue;
                        if (q->unk07 != 0)
                            continue;
                        break;
                    case 2:
                        if (((*mp)->terrain[
                                (*mp)->rowOffset[i] + j] & 0x1f) != 0xd
                         && ((*mp)->terrain[
                                (*mp)->rowOffset[i] + j] & 0x1f) != 0xb)
                            continue;
                        p->x = j;
                        p->y = i;
                        p->v = (s8)gUnknown_03003340[i][j];
                        p++;
                        continue;
                    case 3:
                        if ((*mp)->unit[
                                (*mp)->rowOffset[i] + j] == 0)
                            continue;
                        if (((*mp)->unit[
                                (*mp)->rowOffset[i] + j] & 0xc0)
                                    != gUnknown_03003F2C)
                            continue;
                        q = &gUnknown_08499594[(*mp)->unit[
                                (*mp)->rowOffset[i] + j]];
                        if (q->type > 2)
                            continue;
                        if (q->flags & 8)
                            continue;
                        break;
                    case 4:
                        if ((*mp)->unit[
                                (*mp)->rowOffset[i] + j] == 0)
                            continue;
                        if (((*mp)->unit[
                                (*mp)->rowOffset[i] + j] & 0xc0)
                                    != gUnknown_03003F2C)
                            continue;
                        q = &gUnknown_08499594[(*mp)->unit[
                                (*mp)->rowOffset[i] + j]];
                        if (gUnknown_085767A0[q->type] == 0)
                            continue;
                        if (q->flags & 8)
                            continue;
                        break;
                    default:
                        continue;
                    }
                    p->x = j;
                    p->y = i;
                    p->v = (s8)gUnknown_03003340[i][j];
                    p++;
                }
            }

            p->v = 0xffff;
            p = (struct Unk61308Cell *)gUnknown_03003F20;
            v = sub_08059C00(p, a3);
            if (v == 0x7fff)
                return 0;
            gUnknown_085766E4[k].unk03 = v;
        }
    }
    return 1;
}







