#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026D68.
 * sub_08026D68 @ 0x08026D68
 */

/* The per-army terrain census. Clears each army's tallies, then walks every
 * cell of the gUnknown_08499590 map: the top three bits of the +0x1432 terrain
 * byte select the owning army and the low five are the terrain id, so an owned
 * property adds GetIncomeForTerrainKind(id) to that army's income and bumps one counter
 * per kind. Terrain 8 (the HQ) records its cell instead of counting.
 *
 * The map is reached through a local overlay because gUnknown_08499590 is
 * declared `u8 *`: unk00/unk02 are the width and height, +0x1432 is the cell
 * array and +0x417a the per-row start table, which is the same +0x1432 /
 * rowOffset pair ApplyDailyFuelBurn and AiScoreAttack use.
 *
 * The case bodies are emitted in SOURCE order, and the ROM's block order is
 * 8, 14, 20, 6, 10, 11 -- which is why the switch is written that way rather
 * than in ascending case order. The jump table itself is ascending regardless.
 *
 * The `lsls #0x18` on `b & 0xe0` before the zero test is the u8 local `t`, and
 * `lsrs #0x1d` on that same shifted value is `t >> 5`: read the pair as
 * `(u32)x << 24 >> 29`, a net right shift of 5, not as a mask plus a shift. */
void RecountArmyIncome(void)
{
    int i;
    int j;
    int k;
    u8 b;
    u8 t;
    int kind;
    struct PlayerStruct *army;

    for (i = 0; i <= 4; i++)
    {
        gPlayers[i].income = 0;
        gPlayers[i].bases = 0;
        gPlayers[i].cities = 0;
        gPlayers[i].airports = 0;
        gPlayers[i].ports = 0;
        gPlayers[i].hqX |= 0x80;
    }

    for (j = 0; j < gMap->height; j++)
    {
        for (k = 0; k < gMap->width; k++)
        {
            b = gMap->terrain[
                    gMap->rowOffset[j] + k];

            t = b & 0xe0;
            if (t != 0)
            {
                army = &gPlayers[t >> 5];
                kind = b & 0x1f;

                switch (kind)
                {
                case 8:
                    army->income += GetIncomeForTerrainKind(kind);
                    army->hqX = k;
                    army->hqY = j;
                    break;
                case 14:
                    army->income += GetIncomeForTerrainKind(kind);
                    army->bases++;
                    break;
                case 20:
                    army->income += GetIncomeForTerrainKind(kind);
                    army->labs++;
                    break;
                case 6:
                    army->income += GetIncomeForTerrainKind(kind);
                    army->cities++;
                    break;
                case 10:
                    army->income += GetIncomeForTerrainKind(kind);
                    army->airports++;
                    break;
                case 11:
                    army->income += GetIncomeForTerrainKind(kind);
                    army->ports++;
                    break;
                }
            }
        }
    }

    sub_08026CD0();
}
asm(".global sub_08026D68\n.thumb_set sub_08026D68, RecountArmyIncome\n");
