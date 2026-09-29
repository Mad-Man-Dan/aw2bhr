#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080587FC.
 * sub_080587FC @ 0x080587FC
 */

/* MATCHED in wave 74 (W74-F): configured verdict, size exact at 560/560.
 * The only reported relocation differences are accepted-equivalent force-addr
 * pool words at +0x0c8 and +0x194; promotion must carry the rodata addresses
 * 0x0816D940 and 0x0816D944.
 *
 * The inherited draft was 98.8% with 7 of 560 bytes differing, first at
 * +0x86. Its one real residual was this instruction-scheduling spot:
 *
 *   ROM   lsls r0,r7,#0x10 / mov r2,sb / lsls r1,r2,#0x10 / lsrs r0,r0,#0x10
 *   here  lsls r0,r7,#0x10 / lsrs r0,r0,#0x10 / mov r2,r9 / lsls r1,r2,#0x10
 *
 * Splitting the packed word into `xp = (u32)x << 16`, then
 * `yp = (u32)y << 16`, then `(xp >> 16) | yp` makes the zero-extension two
 * RTL insns and schedules the y shift between them, matching the ROM. This is
 * the same measured lever used by promoted AiMoveToNearestNonTeamCell. Writing the OR the
 * other way round
 * (`(y << 16) | (u16)x`) was TESTED and is worse -- it flips the `orrs` and
 * `str` operands as well, so the current order is right and the `lsrs`
 * position was a scheduling artefact. 300 s of decomp-permuter
 * (~15k iterations) reached exactly this state and no further.
 * Wave 72 tested one further, genuinely distinct spelling: replacing `(u16)x`
 * with `((u32)x << 16) >> 16`. It compiles to the identical 560-byte, 98.8%
 * candidate with the same 7-byte/order residual, confirming this axis is not
 * controlled by the cast's source spelling.
 *
 * The two pool words the diff shows as `.rodata` / `.rodata+4` against the
 * ROM's gUnknown_0816D940 / gUnknown_0816D944 are NOT a defect. I dereferenced
 * baserom.gba: 0x0816D940 holds 0x03003338 and 0x0816D944 holds 0x03003F38,
 * i.e. they are agbcc -fforce-addr copies of &gUnknown_03003338 and
 * &gUnknown_03003F38. Naming the two globals honestly is what makes agbcc
 * emit its own copies into this unit's .rodata, which is the wave-18 placement
 * path; trymatch resolves them once a symbol table exists. Do NOT declare
 * gUnknown_0816D940/944 as globals.
 *
 * Four things each moved this several percent and are all load-bearing:
 *
 *  - `u` is `u8`, not `int`. With `int` the value is kept as `u << 16` across
 *    the calls and every narrow-parameter call site pays a `lsrs r0,r6,#0x10`;
 *    a `u8` local is PROMOTE_MODE zero-extended, so `AreUnitsOnSameTeam(g, u)` and
 *    `IsUnitVisibleToCurrentTeam(u)` need no conversion at all, which is the ROM's bare
 *    `adds r0,r4,#0`.
 *  - the results of AiPickFiringCellBeside and AiPickFiringCellBesideUnit are BOUND to `t` before the
 *    `== -1` test, and AiScoreAttack's is not. That is what decides which of the
 *    two registers holds the result and which holds the -1: bound gives the
 *    ROM's `adds r1,r0,#0 / movs r0,#1 / rsbs r0,r0,#0 / cmp r1,r0`, unbound
 *    gives `movs r1,#1 / rsbs / cmp r0,r1`. Both spellings appear in this one
 *    function and the ROM uses each exactly where written here.
 *  - `r = &gUnits[u];` is bound: inline, the pointer global is
 *    dereferenced before the `u * 12` is computed and the final `adds` operands
 *    swap.
 *  - `lim = gUnknown_030013D0;` before each `*(s16 *)(lim + 0x18)`. Inline off
 *    the `u8 []` declaration agbcc folds the +0x18 into the pool word
 *    (`.word gUnknown_030013D0+0x18` with `movs r3,#0`); the ROM has a clean
 *    `.4byte gUnknown_030013D0` and `movs r3,#0x18`. Same lever c_0804189C.c
 *    documents.
 *
 * `struct Map` and the `pos` word/halfword aliasing are also settled: the
 * planes must be members (else 0x12 folds into ldrb's displacement) and the
 * first write to `pos` really is one word store of `(u16)x | (y << 16)`, which
 * is why it is spelled through the `int *` cast rather than as two `strh`. */

int AiListAttackCandidates(int flag)
{
    struct Unk03003338 *p;
    struct Unit *r;
    struct Unk802C57C pos;
    u8 *lim;
    int w;
    int x;
    int y;
    int idx;
    int t;
    u8 u;
    u32 xp;
    u32 yp;

    p = gUnknown_03003338;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;

            if ((GetMapListEntry(gPlaySt.mapID)->unk28 & 1) == 0
             && gUnknown_020288B4[gMap->rowOffset[y] + x] != 0)
            {
                xp = (u32)x << 16;
                yp = (u32)y << 16;
                *(int *)&pos = (xp >> 16) | yp;
                CalcAttackOutcome(gUnknown_03003F38, 0, &pos);
                lim = (u8 *)gBattleAttacker;
                if (*(s16 *)(lim + 0x18) == 0)
                    continue;
                if (flag == 0)
                {
                    t = AiPickFiringCellBeside(x, y, (u16 *)&pos);
                    if (t == -1)
                        continue;
                }
                else
                {
                    pos.unk00 = gUnknown_030040D8->unk02;
                    pos.unk02 = gUnknown_030040D8->unk03;
                }
                p->unk00 = (x << 8) | y;
                p->unk04 = pos.unk00;
                p->unk06 = pos.unk02;
                p->unk02 = 1;
                p++;
            }
            else
            {
                idx = gMap->rowOffset[y] + x;
                u = gMap->unit[idx];
                if (u == 0)
                    continue;
                if (AreUnitsOnSameTeam(gUnknown_03003F38, u) == 1)
                    continue;
                r = &gUnits[u];
                if (r->type == 0x18 && !IsUnitVisibleToCurrentTeam(u))
                    continue;
                if (!IsCellVisibleToArmy(gUnknown_030033EC, x, y))
                    continue;
                if (flag == 0)
                {
                    t = AiPickFiringCellBesideUnit(u, (u16 *)&pos);
                    if (t == -1)
                        continue;
                }
                else
                {
                    pos.unk00 = gUnknown_030040D8->unk02;
                    pos.unk02 = gUnknown_030040D8->unk03;
                }
                CalcAttackOutcome(gUnknown_03003F38, u, &pos);
                lim = (u8 *)gBattleAttacker;
                if (*(s16 *)(lim + 0x18) == 0)
                    continue;
                if (AiScoreAttack(&w) == -1)
                    w = 0;
                p->unk00 = u;
                p->unk04 = pos.unk00;
                p->unk06 = pos.unk02;
                p->unk02 = w;
                p++;
            }
        }
    }

    p->unk00 = 0;
    return p - gUnknown_03003338;
}
asm(".global sub_080587FC\n.thumb_set sub_080587FC, AiListAttackCandidates\n");
