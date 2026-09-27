#include "global.h"

/* WAVE 88 (W88-A). THE PARK'S CENTRAL CLAIM IS REFUTED AND THIS DRAFT IS A
 * DIFFERENT FUNCTION FROM THE ONE THAT WAS PARKED.
 *
 * The park says sub_0808AAF4 "is the exception in its block; default -O2 is
 * right for it; it gets LONGER under -O1". That was measured with `u16 v`, and
 * `u16 v` is precisely what costs +8 bytes at -O1 (the two `lsl #16; lsr #16`
 * truncation pairs in the loop bodies, which -O2's combine removes and -O1's
 * does not). It is inverted evidence in the wave-16 sense -- a width measured
 * against a body that was still wrong.
 *
 * THE BLOCK'S CONFIGURATION IS `o1-no-force`, NOT `o1`. All sixteen flash-block
 * entries in data/compiler-overrides.json are
 *     cflags_remove ["-O2", "-fforce-addr"], cflags_add ["-O1"]
 * Wave 79's "-O1 branch is ruled out" line for this function was measured under
 * --profile o1, which KEEPS -fforce-addr and is not this block's build. (For
 * this particular function the two profiles happen to emit byte-identical code
 * -- measured -- but the note was not evidence about the block's build.)
 *
 * MEASURED THIS WAVE (each score names its profile):
 *
 *   parked draft (u16 v, no p bind)          configured   152 = 152   59.9%
 *   parked draft + do{}while(0) between loops configured  BYTE-IDENTICAL
 *   int v, no p bind                         o1           152 = 152   64.5%
 *   int v + per-loop `p = &i;`  (this draft) o1           148  (-4)   28.3%
 *   int v + per-loop `p = &i;`               o1-no-force  148  (-4)   28.3%
 *
 * THE do{}while(0) LEVER IS REFUTED HERE, AND USEFULLY. Placed between the two
 * delay loops it changed NOTHING -- not a regression, a no-op. The address
 * pseudo is live across it, so the lever's documented reachable set covered
 * this residual and it still did nothing: at -O2 the two address computations
 * have already been merged by GCSE, and there is only one computation left for
 * the allocator to re-rank.
 *
 * WHAT ACTUALLY MOVED IT: -O1 has no gcse, so nothing merges the two
 * `add r1, sp, #0x40`s, and the whole callee-saved-register residual the park
 * is built around -- `push {r4,r5,r6,lr}`, readFlash1 displaced to r6,
 * `bl _call_via_r6` -- simply does not arise. But -O1 also means loop.c never
 * sees these loops (they are GOTO loops, invisible to it), so nothing hoists
 * the address into the preheaders either; the source has to do it. That is what
 * `vu16 *p; p = &i;` before each loop is for, and it is the OPPOSITE of the
 * un-binding the park asks for.
 *
 * REMAINING DIFF: TWO INSTRUCTIONS, one per loop, and nothing else. Every
 * instruction of both delay loops, both `_call_via_r5` calls, the id assembly
 * and the entire tail are byte-identical to the ROM.
 *
 *   ROM        add r1,sp,#0x40 / ldr r2,=0x00004E20 / adds r0,r2,#0 / b store
 *   candidate  add r1,sp,#0x40 / ldr r0,=0x00004E20                / b store
 *
 * The ROM loads the delay constant into one register and COPIES it into the
 * loop register.
 *
 * WAVE 88 (W88-D) TOOK THIS APART AND THE RESIDUAL IS NOT SOURCE-REACHABLE.
 * The earlier reading -- "agbcc coalesces the two, so find a spelling where the
 * two pseudos CONFLICT" -- is the wrong mechanism, and the lever it implies
 * does not exist. Nothing is being coalesced. What happens is that
 * `update_equiv_regs` gives a SINGLE-USE constant pseudo a REG_EQUIV note,
 * DELETES its defining insn and substitutes the constant into the copy, so the
 * copy stops existing. agbcc has exactly two states here:
 *
 *   ONE use of the constant pseudo -> def deleted, constant substituted:
 *       ldr r0, =0x4E20        (no copy)          push {r4, r5, lr}
 *   TWO uses -> not substituted, both copies survive, but the pseudo is then
 *   live across the two _call_via_r5 calls, becomes a global allocno and takes
 *   a callee-saved register, with the pool load hoisted to the top and done
 *   ONCE:
 *       ldr r6, =0x4E20 ... adds r0, r6, #0   x2  push {r4, r5, r6, lr}
 *
 * The ROM is NEITHER: two copies' worth of non-substitution with one use's
 * worth of locality (a per-loop pool load into a scratch, and no r6). That
 * needs the pseudo denied a hard register so reload rematerialises its
 * REG_EQUIV constant at each use, and with r6 free global.c always prefers the
 * register. Nine spellings are now measured; see work/sub_0808AAF4/W88-notes.md
 * for the table. DO NOT SPEND MORE PROBES ON THE CONSTANT'S SPELLING.
 *
 * Refuted this wave, each an exact no-op (confirming the W80-F bound that a
 * dead constant set and a dead copy are no-ops -- they do not add the
 * reference that would change update_equiv_regs's decision):
 *   n = 20000; v = n; n = 0;      (dead second DEF)  -> no copy
 *   n = 20000; m = n; v = n;      (dead COPY out)    -> no copy
 *   v = 20000; n = v; with n the loop variable       -> no copy, byte-neutral
 *   one n spanning both loops                        -> copies, but r6, 59.9%
 *   *p = 20000; v = *p; (wave 88, W88-A) -> DOES produce the copy but pays an
 *                                 extra strh/ldrh for the volatile round trip
 *                                 (+4 per loop), so it is worse, not better.
 *
 * A `static` (not `static inline`) delay helper is NOT inlined by agbcc -- it
 * emits a real function and the caller then spills r8/r9/sl. Any retry of the
 * helper lever here must say `static inline`.
 *
 * KEPT FROM WAVE 79, still load-bearing: the shared-store goto spelling. The
 * ROM branches INTO the loop body so one `strh` serves both the initialisation
 * and every decrement; `i = 20000; goto test;` emits a separate initialising
 * store and is 4 bytes longer.
 *
 * ALSO STILL TRUE: `_call_via_r5` is a LIFETIME artefact, not an argument
 * count; sub_0808AD6C relocates a 4-byte read routine onto the stack and
 * `sp + 1` is its THUMB entry point.
 *
 * This function should almost certainly carry its block's override (-O1,
 * -fforce-addr removed) like its fifteen siblings. Agents do not edit
 * data/compiler-overrides.json and the standard for an entry is a byte match,
 * so nothing was added.
 */
u16 sub_0808AAF4(void)
{
  u16 buf[0x20];
  u8 (*readFlash1)(u8 *);
  u16 flashId;
  int v;
  vu16 i;
  vu16 *p;
  vu16 *new_var;
  sub_0808AD6C(buf);
  readFlash1 = (u8 (*)(u8 *)) (((s32) buf) + 1);
  *((vu8 *) (0x0E000000 + 0x5555)) = 0xAA;
  *((vu8 *) (0x0E000000 + 0x2AAA)) = 0x55;
  *((vu8 *) (0x0E000000 + 0x5555)) = 0x90;
  p = &i;
  v = 20000;
  goto store1;
  body1:
  v = (*p) - 1;

  store1:
  *p = v;

  if ((*p) != 0)
  {
    goto body1;
  }
  flashId = readFlash1((u8 *) (0x0E000000 + 1)) << 8;
  flashId |= readFlash1((u8 *) 0x0E000000);
  *((vu8 *) (0x0E000000 + 0x5555)) = 0xAA;
  *((vu8 *) (0x0E000000 + 0x2AAA)) = 0x55;
  *((vu8 *) (0x0E000000 + 0x5555)) = 0xF0;
  p = &i;
  v = 20000;
  new_var = &(*p);
  goto store2;
  body2:
  v = (*p) - 1;

  store2:
  *p = v;

  if ((*new_var) != 0)
  {
    goto body2;
  }
  return flashId;
}
