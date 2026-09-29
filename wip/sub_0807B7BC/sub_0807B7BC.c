/* WAVE 87 (W87-F) -- `do { } while (0)` WORKS HERE (the one hit in a batch of
 * five). A wrapper around the whole OUTER while body lifts `outWidths` out of
 * memory into r7 AND `total` into r8, and makes `count` the spilled value --
 * `count` is exactly what the ROM spills. 144 -> 148 bytes against the ROM's
 * 156, i.e. -12 -> -8. Verdict 5.8% (from 9.0%), first difference still +0x2,
 * so by the wave-87 keep rule the draft was RESTORED; the score fall is
 * positional. Wrappers round the Decompress call and round the outTotal tail
 * are byte-neutral; round the inner `for` / the match then-block gives only the
 * outWidths half. WHAT THE LEVER CANNOT DO: every variant still pushes only TWO
 * high registers with an 8-byte frame; the ROM pushes THREE with a 4-byte
 * frame. The wrapper re-ranks which values win the registers already being
 * spent -- it does not change HOW MANY callee-saved registers the function
 * decides to spend. With the wrapper in place the only value still spilled that
 * the ROM keeps in a register is the compiler-invented `str + 1`.
 * See work/sub_0807B7BC/W87-notes.md. */
#include "global.h"
#include "hardware.h"
#include "proc.h"

/* PARKED at 9.0%, candidate 144 bytes against 156 (-12).  Wave 59 (W59-G),
 * first classification of this function.
 *
 * THE RESIDUAL IS NOT A SHAPE.  Every statement, both loop bodies, the
 * Decompress argument computation, both inline constants (0x000003FF and
 * 0x06010000), the `outWidths[count + 1]` operand order (count is the FIRST
 * operand of the `adds`), the u16 truncation of `total` and the `(u16)count`
 * return all come out correct and in the right order.  Two coupled facts are
 * left, and both are allocation/layout.
 *
 *  1. THE REGISTER BUDGET IS ONE SHORT.  The ROM allocates SEVEN call-saved
 *     registers -- r4=str, r5=g, r6=str+1, r7=tile, r8=outWidths, sb=total,
 *     sl=outTotal -- and spills only `count`, into a 4-byte frame
 *     (`str r2,[sp]` / `ldr r2,[sp]` around the Decompress call).  This draft
 *     allocates SIX -- r4=g, r5=str, r6=tile, r7=count, r8=str+1, sb=outTotal
 *     -- and spills BOTH outWidths and total into an 8-byte frame.  Same eight
 *     values either way; the ROM is willing to park two of them in high
 *     registers and pay `mov r0,r8` / `mov r3,sb` at each read, this draft
 *     prefers memory.  The 12 bytes are that third high register's
 *     push/pop/mov (+4), the duplicated outer guard (+4, see 2), and the mov
 *     shuffles (+4).
 *     Note WHY the draft cannot simply be told to use sl: outWidths and total
 *     are both used in `adds`/`lsrs` forms that only encode LOW registers, so
 *     a high register costs a copy at every use and gcc weighs that against a
 *     spill.  This is the allocation class, not a spelling.
 *
 *  2. LOOP ROTATION, AND IT IS SWAPPED BETWEEN THE TWO LOOPS.  The ROM rotates
 *     the OUTER loop -- the test duplicated inline at entry
 *     (`ldrb r0,[r4]; cmp r0,#0; beq _0807B838`) and again at the bottom --
 *     and leaves the INNER loop jump-to-test (`b _0807B82C` into a shared
 *     `cmp r0,#0; bne`).  This draft does exactly the opposite.  Each version
 *     rotates exactly ONE of the two loops and it is the other one.
 *
 * SETTLED READOUTS (do not re-derive):
 *  - FOUR parameters, in r0-r3.  The fifth `void *a5` below is unused, costs
 *    nothing, and is kept only to agree with the existing declaration.
 *  - `sub sp,#4` is a SPILL SLOT for `count`, NOT an outgoing argument slot.
 *    Decompress takes two arguments; r2 is reloaded from [sp] after the call
 *    and used as count, which an argument never would be.
 *  - `count` is `int`, truncated only at the return: `count++` is a bare
 *    `adds r2,#1` with no re-truncation, and the return is `lsls #16; lsrs
 *    #16`.  `total` is genuinely `u16` -- its truncation is stored back into
 *    the variable every iteration.
 *  - `str + 1` is computed at the TOP of the outer body and copied back at the
 *    bottom in BOTH versions, so it is compiler-invented and must not be
 *    authored as a second source variable.
 *
 * RULED OUT, wave 59 (W59-G):
 *  - SOURCE-LEVEL LOOP ROTATION IS NOT A LEVER.  `while (*str != 0) { ...
 *    str++; }` and `if (*str != 0) { do { ... str++; } while (*str != 0); }`
 *    compile to BYTE-IDENTICAL output -- same 144 bytes, same 9.0%, same
 *    registers, same spill slots, same rotation choice, same 130 differing
 *    bytes.  The front end folds the source guard away and a later pass
 *    re-decides the rotation on its own.  Written up as its own chapter in
 *    docs/agbcc-codegen.md; do not spend another attempt spelling the guard.
 *
 * NEXT: the rotation is downstream of the allocation, so there is one fact to
 * move, not two.  What is needed is a source form that makes gcc prefer a
 * third high register over a second spill.  decomp-permuter's documented case
 * is order-wrong/slot-wrong allocation, which this is -- but `mcp permute` was
 * broken in wave 59 (KeyError on 'exit_code', and it strands raw
 * header-expanded output in work/<fn>/<fn>.c, destroying the draft), so that
 * axis is UNTRIED, not ruled out. */
u16 sub_0807B7BC(u8 *str, u16 *outTotal, u8 *outWidths, int tile, void *a5)
{
  int new_var6;
  void *new_var2;
  int new_var4;
  int new_var5;
  int new_var;
  struct Unk08616194 *g;
  int count;
  u16 total;
  u8 new_var3;
  u8 *nx;
  int new_var7;
  total = 0;
  count = 0;
  do
  {
    new_var2 = (void *) 0;
    new_var = 0;
    while ((*str) != new_var)
    {
      do
      {
        nx = str + 1;
        new_var4 = 4;
        new_var5 = 0x06010000;
        for (g = gUnknown_08616194; g->unk00 != 0; g++)
        {
          new_var3 = *str;
          new_var6 = tile & 0x3ff;
          new_var6 = new_var6 << 1;
          if (new_var3 == g->unk00)
          {
            Decompress(g->unk04, (void *) (new_var5 + (new_var6 << new_var4)));
            total += g->unk08;
            if (outWidths != ((void *) 0))
            {
              outWidths[count + 1] = g->unk08;
            }
            new_var7 = 8;
            tile += new_var7;
            count++;
            break;
          }
        }

        str = nx;
      }
      while (0);
    }

    if (outTotal != new_var2)
    {
      *outTotal = total;
    }
    return count;
  }
  while (0);
}
