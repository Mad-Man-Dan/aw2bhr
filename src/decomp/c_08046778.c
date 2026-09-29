#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08046778.
 * sub_08046778 @ 0x08046778
 */

/* Draws every army slot whose terrain-cost row says the slot exists.
 *
 * MATCHED wave 73 (W73-F) by decomp-permuter, run THREE TIMES, each run
 * chained from the previous best.c: 91.67% -> 95.11% -> 99.43% -> match.
 * No single run from the 91.67% draft found any of it, and the function had
 * never been permuted at all before this wave -- its parked note concluded
 * "the lever is NOT the local", which was true of the local it had tried and
 * false of the three the permuter found.
 *
 * DO NOT TIDY THIS BODY. Every one of the four locals below is a live-range
 * split and folding any of them back into its expression loses the match:
 *   - new_var3 binds the row pointer used by the GUARD, while the call
 *     argument keeps the whole chain spelled out. The asymmetry is the point:
 *     wave 49 bound BOTH occurrences to one local and lost 8 bytes, because
 *     that collapses the two address chains the ROM recomputes.
 *   - new_var4 binds the argument's copy IN PLACE, inside the argument.
 *   - new_var / new_var2 split the two u8 inputs of the divide's dividend.
 *   - new_var5 is the dividend as a `short`. It is what makes the divide the
 *     ROM's UNSIGNED `lsrs #3` without a (u32) cast: the value is
 *     u8 + u8 * 8, so 0..2295, and gcc's nonzero_bits on the short proves it
 *     non-negative. The old draft reached the same instruction with
 *     `(u32)(a + gUnknown_084C211C[n * 2] * 8) / 8`; both are unsigned shifts
 *     with no round-toward-zero bias, unlike ClearTerrainInfoMoveCosts's signed divide on
 *     the same table.
 *
 * The movement-cost row is the chain c_08038848.c and c_0801F888.c already
 * spell, and it is written out TWICE per iteration on purpose: the ROM
 * recomputes the whole gPlayers / gUnknown_085D3DD0 address chain for
 * the guard and again for DrawTallNumberRightAligned's fourth argument. That is also why the
 * two occurrences materialise the table base differently -- once as the folded
 * pool constant gen_lds.py names gUnknown_085D3E20
 * (= &gUnknown_085D3DD0[0].unk38[0].unk18[0]) and once as gUnknown_085D3DD0
 * plus `adds #0x50` -- and why the same `s8 *` element load comes out
 * `ldrb; lsls #0x18; asrs #0x18` in the guard and `movs rN,#0; ldrsb` in the
 * argument. One source expression, two expansions.
 */

void DrawTerrainInfoMoveCosts(u8 a, u8 b)
{
  s8 *new_var3;
  u16 i;
  short new_var5;
  u8 new_var2;
  s8 *new_var4;
  u8 new_var;
  u16 n;
  n = 0;
  for (i = 0; i <= 6; i++)
  {
    new_var3 = gUnknown_085D3DD0[(gPlaySt.coAbilities) ? (gPlayers[gUnknown_030033EC].co) : (1)].power[gPlayers[gUnknown_030033EC].coMode].movementChart[gPlaySt.weather];
    if (new_var3[(gUnknown_084C212A[i] * 32) + b] != (-1))
    {
      new_var = gUnknown_084C211C[n * 2];
      new_var2 = a;
      new_var5 = new_var2 + (new_var * 8);
      DrawTallNumberRightAligned(new_var5 / 8, gUnknown_084C211C[(n * 2) + 1], gBG0TilemapBuffer, (new_var4 = gUnknown_085D3DD0[(gPlaySt.coAbilities) ? (gPlayers[gUnknown_030033EC].co) : (1)].power[gPlayers[gUnknown_030033EC].coMode].movementChart[gPlaySt.weather])[(gUnknown_084C212A[i] * 32) + b], 0x8000, 0);
      n++;
    }
  }

  BG_EnableSyncBG0();
}
asm(".global sub_08046778\n.thumb_set sub_08046778, DrawTerrainInfoMoveCosts\n");
