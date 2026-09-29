#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059F24.
 * sub_08059F24 @ 0x08059F24, sub_0805A008 @ 0x0805A008
 */

/* WAVE 59 (W59-C). MATCH. Drafted since wave 44, diagnosed in wave 52, and
 * still open at 31.6% / +4 bytes at the start of this wave. Two independent
 * facts closed it; both are now chapters in docs/agbcc-codegen.md.
 *
 * 1. THE ZERO-BYTE LICM BARRIER (wave 52, W52-B), AND WHY IT WAS MISSED FOR
 *    SIX WAVES. The chapter "A store through a POINTER PARAMETER does not kill
 *    a global pointer's load" ends by saying what is left to test is
 *    "something that makes the cursor's base untraceable to an argument".
 *    That framing treats the CURSOR as the only candidate barrier, and it is
 *    the wrong object. prescan_loop's note_addr_stored is not a per-object
 *    fact: ONE store the compiler cannot trace makes invariant_p return 0 for
 *    EVERY MEM in that loop. The barrier does not have to be the out-pointer.
 *
 *        gUnknown_03003340[y][x] += 0;
 *
 *    stores through a row pointer LOADED FROM MEMORY -- W52-C's `t2` case. It
 *    is dead and is deleted before output, so it costs ZERO bytes, and on its
 *    own it takes this function from 31.6% / +4 to 87.3% / SIZE-EXACT: the
 *    duplicated loop guard collapses to the ROM's `ldr r1,[pc,#4];
 *    ldr r0,[r1]; b _08059FD4` (the ROM's dead load included),
 *    gUnknown_08499590 is re-read at all four use sites, and ip goes back to
 *    holding the invariant `y * 2` instead of spilling it -- the whole +4.
 *
 * 2. The remaining 29 bytes were pure register allocation, closed in three
 *    steps, each verified separately:
 *      87.3 -> 94.3  `do { } while (0)` around the last test and the stores.
 *                    This is what swaps `out` into r3 and `x` into r4.
 *      94.3 -> 98.2  bind `u = &gUnits[cell]` in its OWN statement.
 *                    That fixes the evaluation ORDER inside the `unk00` test:
 *                    the ROM computes cell*12 FIRST and loads the pointer
 *                    second, so the destination is the operand computed LAST
 *                    (`adds r1,r1,r0`, not `adds r0,r0,r1`).
 *      98.2 -> MATCH `(off = cell)` inside the gUnknown_03004730 subscript.
 *                    The last 4 bytes were which of two address constants,
 *                    each used once in the loop, wins the callee-saved
 *                    register: the ROM hoists &gUnits into sl and
 *                    rematerialises gUnknown_03004730 from the pool, and the
 *                    draft did the opposite (which is also why pool slots +0x8
 *                    and +0x14 were swapped -- ONE fact, not two). This is the
 *                    wave-17 lever exactly: a reference created at a point no
 *                    statement boundary can reach, reordering the allocnos.
 *
 * THREE PERMUTER RUNS, EACH STARTED FROM THE PREVIOUS BEST -- that is the
 * pattern to copy, not a 300s run from a cold draft. 87.3 -> 94.3 (found the
 * do-while) -> 98.2 (found the `u =` binding) -> match in 119 iterations. The
 * tool's own hint says it: "a different starting point usually helps more".
 *
 * DO NOT TIDY `new_var` AWAY. Folding `new_var = ...; off = new_var;` back into
 * one statement drops this straight to 98.2%. Tested this wave, on the matched
 * file, exactly the regression the wave-52 chapter warns about. Re-run
 * try_match after ANY cosmetic edit here.
 *
 * MEASURED AND DEAD, so nobody re-spends them:
 *   - `(&gUnits[cell])->unk00` written INLINE is byte-identical to
 *     `gUnits[cell].type`. It is the separate STATEMENT that
 *     matters here, not the `&`.
 *   - Moving the `out->v` store ahead of `out->x`/`out->y` re-breaks the
 *     barrier, restores the duplicated guard AND the spill, and turns the map
 *     read into `ldrsb`. The store order is load-bearing.
 *   - Putting the `unk00` test inside the do-while as well is byte-neutral.
 *   - The parameter cannot be `struct Unk5A514Cell *`:
 *     include/unknown-functions.h declares `void AiListUnescortedFootUnits(void *)`.
 *
 * AiListUnescortedLanders is this shape with ONE compare changed (`!= 0x17` for `> 2`)
 * and matched on the first attempt from this derivation. */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void AiListUnescortedFootUnits(void *a1)
{
  struct Unk5A514Cell *out;
  struct Unit *u;
  int new_var;
  u8 cell;
  int x;
  int y;
  int off;
  out = a1;
  for (y = 0; y < gMap->height; y++)
  {
    for (x = 0; x < gMap->width; x++)
    {
      gUnknown_03003340[y][x] += 0;
      if (((s8) gUnknown_03003340[y][x]) < 0)
      {
        continue;
      }
      new_var = gMap->rowOffset[y] + x;
      off = new_var;
      cell = gMap->unit[off];
      if (cell == 0)
      {
        continue;
      }
      if ((cell & 0xc0) != gUnknown_03003F2C)
      {
        continue;
      }
      u = &gUnits[cell];
      if (u->type > 2)
      {
        continue;
      }
      do
      {
        if (gUnknown_03004730[(off = cell) & 0x3f] != 0)
        {
          continue;
        }
        out->x = x;
        out->y = y;
        out->v = (s8) gUnknown_03003340[y][x];
        out++;
      }
      while (0);
    }

  }

  out->v = 0xFFFF;
}
asm(".global sub_08059F24\n.thumb_set sub_08059F24, AiListUnescortedFootUnits\n");

void AiListUnescortedLanders(void *a1)
{
  struct Unk5A514Cell *out;
  struct Unit *u;
  int new_var;
  u8 cell;
  int x;
  int y;
  int off;
  out = a1;
  for (y = 0; y < gMap->height; y++)
  {
    for (x = 0; x < gMap->width; x++)
    {
      gUnknown_03003340[y][x] += 0;
      if (((s8) gUnknown_03003340[y][x]) < 0)
      {
        continue;
      }
      new_var = gMap->rowOffset[y] + x;
      off = new_var;
      cell = gMap->unit[off];
      if (cell == 0)
      {
        continue;
      }
      if ((cell & 0xc0) != gUnknown_03003F2C)
      {
        continue;
      }
      u = &gUnits[cell];
      if (u->type != 0x17)
      {
        continue;
      }
      do
      {
        if (gUnknown_03004730[(off = cell) & 0x3f] != 0)
        {
          continue;
        }
        out->x = x;
        out->y = y;
        out->v = (s8) gUnknown_03003340[y][x];
        out++;
      }
      while (0);
    }

  }

  out->v = 0xFFFF;
}
asm(".global sub_0805A008\n.thumb_set sub_0805A008, AiListUnescortedLanders\n");
