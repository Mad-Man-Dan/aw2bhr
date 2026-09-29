#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E440.
 * sub_0805E440 @ 0x0805E440, sub_0805E5AC @ 0x0805E5AC
 */

/* Wave 52, W52-D.  MATCHED (364/364, relocs resolve to the same addresses).
 *
 * gUnknown_0816DA6C / _0816DA70 are NOT globals -- the ROM words there hold
 * 0x030040D8 and 0x08499590, so they are agbcc's own -fforce-addr constants for
 * gUnknown_030040D8 and gUnknown_08499590.  Promotion must carry them:
 *   "rodata": ["0x0816DA6C", "0x0816DA70"]
 *
 * Both `unk04` reads are 7-BIT BITFIELDS and the ROM reads each of them TWICE
 * off one `ldrb` with two different extractions -- `movs r0,#0x7f; ands` where
 * it is only tested against zero, and `lsls #0x19; lsrs #0x19` where the value
 * reaches Div.  That is exactly the discriminating evidence recorded on
 * struct Unk030040D8's unk04 and struct Unit's unk04_0, and both already
 * carry it, so nothing needed declaring.
 *
 * THE ONE THING THAT COST ANYTHING: `n + 1 + Div(...)`.  agbcc's fold()
 * reassociates it to `n + (Div(...) + 1)` and emits `add rQ,rQ,#1; add rQ,rN,rQ`
 * where the ROM has `adds r1,r5,#1; adds r1,r1,r0`, i.e. `(n + 1) + Div(...)`.
 * Measured on four spellings in one probe: `n + 1 + q`, `q + (n + 1)` and
 * `q = Div(...); n + 1 + q` ALL fold to `n + (q + 1)`; only a STATEMENT BOUNDARY
 * around `n + 1`, placed AFTER the Div statement, keeps the ROM's grouping.
 * This is the wave-35 (W35-A) `A - (B - K)` finding on a PLUS: parentheses do
 * not stop fold's associate, and neither does binding the call's result -- only
 * splitting the constant's addition into its own statement does.
 *
 * The `x << 16` in r8 is strength_reduce's giv for the `(s16)x` that
 * IsCellCapturableByCurrentArmy's declared parameters force; it is not authored. */

void AiTryJoinUnitOnProperty(void)
{
    int x;
    int y;
    int id;
    int n;
    struct Unit *u;

    GenerateUnitMovementMap(gUnknown_030040D8);

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;
            id = gMap->unit[gMap->rowOffset[y] + x];
            if (id == 0)
                continue;
            if ((id & 0xc0) != gUnknown_03003F2C)
                continue;
            if (IsCellCapturableByCurrentArmy(x, y) == 0)
                continue;
            u = gUnits + id;
            if (u->hp != 0)
            {
                if (Div(u->hp - 1, 10) == 9)
                    continue;
            }
            if (u->type != gUnknown_030040D8->unk00)
                continue;
            if (u->hp != 0)
                n = Div(u->hp - 1, 10) + 1;
            else
                n = 0;
            if (gUnknown_030040D8->unk04 != 0)
            {
                int q;
                int t;

                q = Div(gUnknown_030040D8->unk04 - 1, 10);
                t = n + 1;
                if (t + q > 10)
                    continue;
            }
            else
            {
                if (n > 10)
                    continue;
            }
            AiPublishAction(x, y, 10, 0, 0);
        }
    }
}
asm(".global sub_0805E440\n.thumb_set sub_0805E440, AiTryJoinUnitOnProperty\n");

void AiStayHomeIfEnemyInfantryNear(void)
{
    s16 x;
    s16 y;
    int id;
    struct Unit *u;

    if ((gMap->terrain[gMap->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02] & 0xe0) != gUnknown_03004084)
        return;
    if (gUnknown_085767B8[gMap->terrain[gMap->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02] & 0x1f] != 0)
        return;

    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03, 1, 3, 0);

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;
            if (gMap->unit[gMap->rowOffset[y] + x] != 0)
            {
                u = gUnits + (id = gMap->unit[gMap->rowOffset[y] + x]);
                if (AreUnitsOnSameTeam(gUnknown_03003F38, u - gUnits) == 0)
                {
                    if (u->type == 1)
                        AiPublishAction(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03, 2, 0, 0);
                }
            }
        }
    }
}
asm(".global sub_0805E5AC\n.thumb_set sub_0805E5AC, AiStayHomeIfEnemyInfantryNear\n");
