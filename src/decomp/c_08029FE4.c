#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029FE4.
 * sub_08029FE4 @ 0x08029FE4
 */

/* MATCHED -- wave 60 (W60-D).  512/512 bytes.
 *
 * PROMOTION REQUIREMENT (unchanged from the wave-56 park, and confirmed by
 * trymatch): this match needs its .rodata pool words PLACED.  The
 * data/promoted.json entry must carry
 *     "rodata": ["0x08090B74", "0x08090B78"]
 * and tools/split_rodata.py + tools/gen_lds.py must be re-run before building.
 * trymatch reports +0xe8/+0xec as "different symbols that resolve to the same
 * address" and calls it a match; that is the -fforce-addr address-constant
 * pool for gUnknown_03001470 and gUnknown_03001FBC, exactly as the wave-56
 * park predicted.
 *
 * HOW IT CLOSED, from the wave-56 park's 91.2% size-exact draft.  Three edits,
 * two of them found by decomp-permuter (shell tools/permute.py, run REPEATEDLY
 * with each round starting from the previous round's best.c: 91.2 -> 96.7 ->
 * 99.4 -> 99.8; a single long run from the draft found nothing, replicating
 * wave 59's 87.3 -> 94.3 -> 98.2 -> match):
 *
 *  1. THERE IS NO `cell` LOCAL.  `gMap->terrain[idx]` is read inline at
 *     both uses -- `(gMap->terrain[idx] & 0xE0)` and `gMap->terrain[idx] & 0x1F`;
 *     a `cell` pseudo took r4 ahead of `u` and rotated r3/r4/r5.
 *
 *  2. The row/cell/mark planes are read straight through gMap->rowOffset[],
 *     gMap->terrain[] and gMap->unk234A[], with no pointer locals bound.
 *
 *  3. THE TWO-CALL SUM MUST BE SPLIT, AND 978 EVALUATED FIRST:
 *         n = ResupplyUnitAmmo(u, 0);
 *         n = ResupplyUnitFuel(u, 0) + n;
 *     The single expression `ResupplyUnitAmmo(u,0) + ResupplyUnitFuel(u,0)` emits the
 *     right instruction shape but the WRONG call order.  Note this difference
 *     costs ZERO bytes -- both `bl`s encode as offset 0 plus a relocation -- so
 *     it is invisible in the byte score and only the reloc comparison catches
 *     it.  A 99.8%-identical candidate can still have two calls swapped.
 *
 *  4. `u8 flag`, NOT `int flag`.  This is the last two bytes and it is a
 *     general rule (written up in docs/agbcc-codegen.md).  StartSupplyAnimation's
 *     fourth parameter is `u8` -- and that is right, it agrees with the
 *     promoted definition in src/decomp/c_08029C38.c.  With an `int` local the
 *     int->u8 conversion at the call emits no instruction of its own (gcc
 *     proves it redundant, flag is 0 or 1) but it REORDERS the argument setup
 *     to `mov r3, ip ; mov r0, sp`, where the ROM has `mov r0, sp ; mov r3,
 *     ip`.  Declaring the local at the parameter's own width removes the
 *     conversion node and restores the order.
 *
 * A TRAP WORTH KNOWING (wave 60, W60-D): decomp-permuter's type randomizer
 * MUTATES PROTOTYPES inside its own expanded translation unit.  Its 99.8%
 * best.c had silently rewritten StartSupplyAnimation's fourth parameter from `u8` to
 * `int` -- which reproduces the ROM's argument order, but is a change we
 * cannot legally adopt, because the promoted body in src/decomp/c_08029C38.c
 * defines it `u8`.  The legal spelling with the SAME effect is the `u8 flag`
 * local above.  Always diff work/<fn>/permuter/base.c against include/ before
 * believing a permuter win: base.c is regenerated from best.c each round, so
 * once a prototype mutation is saved it is baked into every later round.
 *
 * STILL SETTLED FROM WAVE 56, do not re-derive:
 *  - `p7 = &gUnknown_03004007;` at the top of the loop body is LOAD-BEARING;
 *    it is the only reason sl holds the right symbol.  Its position does not
 *    matter, only its presence.
 *  - `n != 0 || m != 0` is the right spelling of the early-out, and the
 *    `if (m != 0)` arm really does zero `n` before StartSupplyAnimation.
 *  - the `gMap->unk234A[idx] == 0` arm (three calls, results discarded) must be the
 *    fall-through, and both arms are genuine source call sites.
 *  - `pt` is a 4-byte struct, so both member stores are SImode bitfield
 *    inserts on one stack word.
 */
void TurnStartRepair_Loop(void)
{
  struct Unit *u;
  struct Unk802C57C pt;
  u8 *p7;
  u16 i;
  int idx;
  u32 save;
  int n;
  int m;
  u8 flag;
  if (FindSlotScript((s32) gUnknown_0849A0A8) != (-1))
  {
    return;
  }
  i = gUnknown_03001470[gUnknown_03001FBC].unk38;
  for (; i <= 0x32; i++)
  {
    p7 = &gUnknown_03004007;
    u = &gUnits[gUnknown_03003F2C + i];
    if (u->type == 0)
    {
      continue;
    }
    if ((u->flags & 9) != 0)
    {
      continue;
    }
    idx = gMap->rowOffset[u->y] + u->x;
    ;
    if ((gMap->terrain[idx] & 0xE0) != gUnknown_03004084)
    {
      continue;
    }
    if (gUnknown_085D5ABC[u->type].repairTable[gMap->terrain[idx] & 0x1F] == 0)
    {
      continue;
    }
    if (gMap->unk234A[idx] == 0)
    {
      ResupplyUnitAmmo(u, 0);
      ResupplyUnitFuel(u, 0);
      RepairUnit(u, 2, 1 - (*p7));
    }
    else
    {
      save = gPlayers[gUnknown_030033EC].funds;
      n = ResupplyUnitAmmo(u, 0);
      n = ResupplyUnitFuel(u, 0) + n;
      m = RepairUnit(u, 2, 1 - (*p7));
      if ((n != 0) || (m != 0))
      {
        flag = 0;
        if (m != 0)
        {
          flag = 1;
          gPlayers[gUnknown_030033EC].funds = save;
          n = 0;
        }
        pt.unk00 = u->x;
        pt.unk02 = u->y;
        StartSupplyAnimation(&pt, gUnknown_030033EC, m + n, flag);
        gUnknown_03001470[gUnknown_03001FBC].unk38 = i + 1;
        break;
      }
    }
  }

  if (i == 0x33)
  {
    sub_08029FC4();
    ClearSlotScriptCallback(gUnknown_03001FBC);
  }
}
asm(".global sub_08029FE4\n.thumb_set sub_08029FE4, TurnStartRepair_Loop\n");
