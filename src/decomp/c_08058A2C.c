#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08058A2C.
 * sub_08058A2C @ 0x08058A2C
 */

#include "map.h"

/*
 * AiScoreAttack -- AI: score the battle that was just simulated.
 *
 * Reads gBattleAttacker and gBattleDefender. Returns -1 at once if the
 * attacker's HP loss (plus 50 if it would be destroyed) reaches the limit at
 * *gUnknown_03004784.
 *
 * Otherwise it weights the defender:
 *   - Infantry-class defenders (type 2 or lower) on a property tile that
 *     IsTerrainOwnedByUnitsTeam rejects: weight 1, or 32 on terrain kind 8, times 8 on
 *     kinds 0xA, 0xB and 0xE; then times (unk05_3 + displayed HP) / 5 + 1,
 *     times 100.
 *   - Everything else: weight 100 or 150, by sub_08042D50.
 *   - Units whose type data has unk1b == 2 are doubled if unk08 is set, or
 *     divided by 8 if unk07 is 0.
 * The score is (defender HP loss, at least 50 if destroyed) * (weight / 16)
 * minus 6 * the attacker's HP loss. It is stored in *out, and the function
 * returns -1 if it is negative, else 0.
 *
 * Why the C looks odd:
 *   - The terrain kind is read into its own local, `kind`, through the full
 *     map index. Reusing `n` or `off` puts the values in different registers.
 */

int AiScoreAttack(int *out)
{
    struct Unit *u;
    int atk;
    int def;
    int v;
    int off;
    int n;
    int t;
    int d;
    int t2;
    int res;
    int kind;

    v = 1;
    u = gBattleDefender->unit;
    atk = gBattleAttacker->hpLoss;
    if (gBattleAttacker->remainingHp == 0)
        atk += 0x32;
    if (atk >= *gUnknown_03004784)
        return -1;

    if (u->type <= 2)
    {
        off = ((struct Map *)gUnknown_08499590)->rowOffset[u->y] + u->x;
        if (((struct Map *)gUnknown_08499590)->property[off] != -1)
        {
            if (!IsTerrainOwnedByUnitsTeam(((struct Map *)gUnknown_08499590)->unit[off], ((struct Map *)gUnknown_08499590)->terrain[off]))
            {
                kind = ((struct Map *)gUnknown_08499590)->terrain[((struct Map *)gUnknown_08499590)->rowOffset[u->y] + u->x] & 0x1F;
                if (kind == 8)
                    v = 0x20;
                if (kind == 0xE || kind == 0xA || kind == 0xB)
                    v = v * 8;
                t = u->unk05_3;
                if (u->hp != 0)
                {
                    d = Div(u->hp - 1, 10);
                    t2 = t + 1;
                    n = t2 + d;
                }
                else
                    n = t;
                v *= n / 5 + 1;
                v *= 100;
                goto merge;
            }
        }
    }
    v = v * 10;
    if (sub_08042D50(gUnknown_030033EC, u->type) == 1)
        v = v * 10;
    else
        v = v * 15;

merge:
    def = gBattleDefender->hpLoss;
    if (gBattleDefender->remainingHp == 0 && def <= 0x31)
        def = 0x32;
    if (gUnknown_085D5ABC[u->type].unk1b == 2)
    {
        if (u->unk08 != 0)
            v = v * 2;
        else if (u->unk07 == 0)
            v = v >> 3;
    }
    def = def * (v >> 4);
    atk = atk * 6;
    res = def - atk;
    *out = res;
    if (res < 0)
        return -1;
    return 0;
}
asm(".global sub_08058A2C\n.thumb_set sub_08058A2C, AiScoreAttack\n");
