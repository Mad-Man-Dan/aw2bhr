#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029DF8.
 * sub_08029DF8 @ 0x08029DF8
 */

/* MATCHED, wave 56 (W56-E).  Walks the four neighbour directions of the cursor
 * (gUnknown_0849A0D8 is a 3 x s16 record per direction: a bit mask, dx, dy),
 * finds the first neighbour holding a unit that can still act, and starts it.
 * proc->unk1e is the resume cursor, so the whole thing is a coroutine step.
 *
 * FIVE readouts, each of which was worth an attempt:
 *
 *  1. LOOP SHAPE.  `for (;;) { if (done) { cleanup; return; } body; }`, NOT
 *     `while (!done) { body } cleanup;`.  The ROM branches FORWARD to the body
 *     and falls through into the cleanup, which only the early-exit `if`
 *     produces; the `while` form lays the cleanup after the loop and inverts
 *     every branch in the function.
 *
 *  2. The inner test is spelled with `||` and the ELSE-arm falling through:
 *     `if (gMap->unk234A[idx2] == 0 || IsCellOnScreen(...) == 0) CalcUnitResupplyCost(u); else
 *     {...}`.  Written as `&&` with the arms the other way round, agbcc puts
 *     the one-call arm behind the pool and jumps to it.  Both arms really do
 *     call CalcUnitResupplyCost -- that is two call sites in the source, not a
 *     duplicated one.
 *
 *  3. IsCellOnScreen takes TWO arguments and only ONE is set up
 *     (`adds r0, r3, #0; bl`).  r1 already holds u->y from the idx2
 *     computation, so the second argument is free -- the wave-51 arity rule.
 *     Reading it as a one-argument call is the trap here.
 *
 *  4. TWO wave-17 ANCHORS, and the function does not close without either.
 *     There are three loop-invariant address constants and two callee-saved
 *     hi registers, so the allocation is decided by which pseudo is created
 *     first, i.e. by source order:
 *       - `pp = &gUnits;` in the PREHEADER (after the guard, before
 *         the loop) is c_08026100.c's idiom exactly.  Without it agbcc hoists
 *         &gUnknown_03003100 instead and gUnits gets an inline pool
 *         word every iteration.
 *       - `ec = &gUnknown_030033EC;` as the first statement of the else-arm,
 *         BEFORE the CalcUnitResupplyCost call.  Its only read is the last argument
 *         setup of StartSupplyAnimation, so nothing else makes the address live
 *         across the call, and without it the second hi register is never
 *         allocated at all (`pop {r3}` instead of `pop {r3, r4}`).
 *       - `cp = &gUnknown_03003100.pos;` right after `map = ...`, which fixes
 *         r6/r7.
 *
 *  5. OPERAND ORDER of a commutative `+` is NOT source order when the two
 *     operands differ in complexity -- gcc's commutative_operand_precedence
 *     swaps them, and the swap flips which register the `adds` writes.
 *     Two places, 16 bytes together:
 * *       - the pt stores must be written `cp->unk00 + table[...]`, i.e. the
 *         OPPOSITE of the reading order, to get the ROM's `ldrh table` first.
 *     The row lookups read gMap->rowOffset[] directly.
 *
 * `pt` is a 4-byte struct, so agbcc gives it SImode and both member stores
 * become read-modify-write bitfield inserts on one stack word
 * (`ldr; ands; orrs; str`), not two `strh`.  That is expected output for any
 * 4-byte struct local, not a packed-word idiom in the source.
 *
 * The table reads that feed int arithmetic are `(s16)` casts and emit `ldrsh`;
 * the two that feed pt's u16 members emit a plain `ldrh` from the identical
 * spelling, because only the low half is live there. */
void SupplyCommand_Loop(struct Unk03001470 *proc)
{
  struct Unit **pp;
  struct Unit *u;
  struct Unk802C57C *cp;
  struct Unk802C57C pt;
  u16 *ec;
  int idx;
  int idx2;
  int c;
  int new_var;
  struct Map *new_var2;
  if (FindSlotScript((s32) gUnknown_0849A0A8) != (-1))
  {
    return;
  }
  pp = &gUnits;
  for (;;)
  {
    if (proc->unk1e > 3)
    {
      if (proc->unk20 == 0)
      {
        gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
        gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;
      }
      EndSlotScriptAt(gUnknown_03001FBC);
      CommitUnitMove();
      if ((proc->unk20 == 0) && (gPlaySt.savingEnabled != 0))
      {
        SendActionCommand(6, gUnknown_03003F38, 0, 0);
      }
      LockUnitSelection();
      DecrementMapLock();
      return;
    }
    if ((gUnknown_03003F40 & ((s16) gUnknown_0849A0D8[proc->unk1e * 3])) != 0)
    {
      new_var2 = gMap;
      cp = &gUnknown_03003100.pos;
      idx = new_var2->rowOffset[((s16) gUnknown_0849A0D8[(proc->unk1e * 3) + 2]) + cp->unk02] + (((s16) gUnknown_0849A0D8[(proc->unk1e * 3) + 1]) + (*cp).unk00);
      u = &(*pp)[new_var2->unitUnk[idx]];
      new_var = 0;
      idx2 = new_var2->rowOffset[u->y] + u->x;
      if ((new_var2->unk234A[idx2] == new_var) || (((u8) IsCellOnScreen(u->x, u->y)) == 0))
      {
        CalcUnitResupplyCost(u);
      }
      else
      {
        ec = &gUnknown_030033EC;
        c = CalcUnitResupplyCost(u);
        if (c != 0)
        {
          pt.unk00 = cp->unk00 + gUnknown_0849A0D8[(proc->unk1e * 3) + 1];
          pt.unk02 = cp->unk02 + gUnknown_0849A0D8[(proc->unk1e * 3) + 2];
          StartSupplyAnimation(&pt, *ec, c, 0);
          proc->unk1e++;
          return;
        }
      }
    }
    proc->unk1e++;
  }

}
asm(".global sub_08029DF8\n.thumb_set sub_08029DF8, SupplyCommand_Loop\n");
