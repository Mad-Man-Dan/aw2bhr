#include "global.h"

/* WAVE 87 (W87-C) -- CLEAN NO, draft unchanged at 120/124, 23.4%
 * (`configured`). No probe and no try_match spent. Evidence in
 * work/sub_0803A2BC/W87-notes.md.
 *  - TWIN AXIS, negative. The screened twins sub_080059FC and sub_08005AA0
 *    (src/decomp/c_080059FC.c) share only the callee sub_08014A5C and the
 *    global gUnknown_08499578. Both are `void f(void)`; all eleven of their
 *    sub_08014A5C calls pass integer LITERALS in argument 1. There is no
 *    computed argument anywhere in either function, so they contain no
 *    instance of this residual's `(x + 8) >> 3` head at all.
 *  - PROTOTYPE AXIS, negative. include/unknown-functions.h:6450 declares
 *    `void sub_08014A5C(int, int, void *, int, int, int)` -- all six wide,
 *    read off the callee's own prologue -- and the promoted callers agree.
 *    Argument 1 is `int`, so `(a1 + 8u) >> 3` is passed with NO narrowing and
 *    there is no prototype/definition disagreement to exploit.
 * Do not batch this on the shared callee again: sub_08014A5C /
 * gUnknown_08499578 is a UI-drawing hub with dozens of literal-argument
 * callers and carries no information about this residual. */


/* WAVE 79 (W79-C) -- RE-MEASURED AND RECLASSIFIED. READ THIS BEFORE THE
 * WAVE-35/43 BLOCK BELOW, WHICH QUOTES A SCORE THAT IS NOT THIS FILE'S.
 *
 * THIS DRAFT measures 120 bytes against 124 (-4), 23.4% identical, first
 * difference at +0xc. The 78.2% that data/parked.json and trymatch quote is
 * best.c's, and best.c is permuter output with the signature
 * `sub_0803A2BC(unsigned long long a1, ...)` and header-expanded prototypes:
 * size-exact at 124, but not promotable and not a draft.
 *
 * The parked.json entry used to say nobody had ever worked this function. That
 * was true of the ENTRY, never of the function -- waves 35 and 43 worked it and
 * their evidence is the comment block below. The entry has been rewritten.
 *
 * THE RESIDUAL IS ONE INSTRUCTION. The ROM's head is SEVEN instructions and
 * every candidate emits SIX, because the ROM has THREE pseudos where the
 * candidate has one: `(u8)a1` dies immediately into a call-clobbered r1,
 * `a1 + 8` dies immediately, and only the `>> 3` result lives across the two
 * calls in r4. The candidate does the `+ 8` and the `>> 3` IN PLACE on the
 * parameter's own promoted pseudo, so that pseudo takes r4 at once, r1 never
 * has to be freed, a2 is therefore copied SECOND (into r7) and the 0x8000
 * constant takes r6 -- where the ROM has a2 in r6 and 0x8000 in r7. That is
 * why every later register name is one off. The a2/0x8000 pair is a SYMPTOM of
 * the head, not an independent second fact.
 *
 * WAVE 79 RULED OUT, nine spellings, ALL byte-identical to this draft's
 * six-instruction head (compile_probe):
 *   - u32 y; y = a1; y += 8; y >>= 3;            (in place, multi-set pseudo)
 *   - u32 y, i; y = a1 + 8u; i = y >> 3;
 *   - u32 y; y = a1 + 8u; then `y >> 3` inline at BOTH calls
 *   - u8 c; u32 y; u32 i;  three single-assignment locals
 *   - u32 i; i = (a1 + 8u) >> 3;  bound once
 *   - the asymmetric best.c shape written honestly: `y = a1 + 8u` used by one
 *     call, `(a1 + 8u) >> 3` inline at the other
 *   - u8 v = a2->unk00 hoisted   (moves the `ldrb` EARLIER -- wrong direction)
 *   - struct Unit *p = a2 alias
 *   - u32 a1 with ((u8)a1 + 8u) >> 3
 *
 * CLASSIFICATION: a COPY-COALESCING decision with NO SOURCE HANDLE. Every C
 * spelling of a single-use chain lets local_alloc coalesce the three values
 * into the parameter's pseudo, and `lsrs rD,rS,#imm5` is a 3-operand form, so
 * the ROM's `adds r0,r4,#0` before the shift is a non-coalesced copy that no
 * correct allocation would emit and no expression tree can request. Under the
 * wave-77 screen this is "register numbers with nothing behind them".
 *
 * The named exemplar src/decomp/c_0803A338.c is the matched CALLER
 * (`sub_0803A2BC(gUnknown_0849D89C->unk00, gUnknown_0849D89C->unk04)`). It
 * confirms the parameter types and can address nothing else.
 *
 * DO NOT spend another wave's budget on the head. -- W79-C
 */

/* PARKED. Wave 35 (W35-E) left this at 120/124 bytes; wave 43 (W43-I) ran the
 * permuter axis W35-E named and it moved the problem, so the park reason below
 * has CHANGED and one of W35-E's conclusions is refuted.
 *
 * EVERYTHING FROM THE FIRST POOL LOAD ONWARD IS INSTRUCTION-EXACT -- both
 * sub_08014A5C calls, the 0x5c stride kept in r8, the gUnknown_085D5ABC and
 * gUnknown_08499578 bases in sl/sb, the 0x8000 and the 0 kept across the pair,
 * and the `ldrh` at +2 / +4 (which is why struct UnitType now has unk02 and
 * unk04 carved out of its old filler_00[6]).
 *
 * THE WHOLE REMAINING DIFF IS THE FIRST ARGUMENT'S SEVEN-INSTRUCTION HEAD:
 *
 *   ROM:  adds r6, r1, #0      ; a2 copied FIRST, a1 untouched
 *         lsls r0, r0, #0x18
 *         lsrs r1, r0, #0x18   ; (u8)a1 lands in its own register
 *         adds r4, r1, #0      ; ... and is COPIED before the +8
 *         adds r4, #8
 *         adds r0, r4, #0      ; ... and copied again before the shift
 *         lsrs r4, r0, #3
 *   here: adds r4, r0, #0      ; a1 copied first
 *         adds r7, r1, #0
 *         lsls r4, r4, #0x18
 *         lsrs r4, r4, #0x18
 *         adds r4, #8
 *         lsrs r4, r4, #3
 *
 * `lsrs` (not `asrs`) is settled: the shift operand must be unsigned, so
 * `(a1 + 8) >> 3` on the plain u8 parameter is wrong and `(a1 + 8u) >> 3` is
 * right. That fix alone took this from 168 bytes to 120.
 *
 * RULED OUT with compile_probe, all byte-identical to this draft:
 *   - u32 i; i = a1 + 8; ... i >> 3
 *   - u8 v; u32 w; int i; v = a1; w = v + 8; i = w >> 3;   (three locals)
 *   - those three locals DECLARED IN REVERSE ORDER
 *   - int a1 with the cast at the use: ((u8)a1 + 8u) >> 3
 *
 * WAVE 43 (W43-I) -- THE PERMUTER WAS RUN, AND W35-E'S "agbcc coalesces all
 * three into r4 FOR EVERY SPELLING" IS WRONG. Three runs, ~1500 s and ~140k
 * iterations, hill-climbing 63.7% -> 74.2% -> 78.2% and then flat ("no
 * candidate scored better than the starting point"). EVERY candidate from the
 * first run onward is SIZE-EXACT at 124 bytes. So the two missing instructions
 * are reachable from C -- the three pseudos DO survive uncoalesced -- and this
 * is no longer a "can the extra instructions be produced" question at all.
 *
 * The shape that produces the seven-instruction head is to compute `a1 + 8u`
 * into its own variable used by ONE call while the other call spells
 * `(a1 + 8u) >> 3` inline, i.e. the value is a variable on one side and an
 * expression on the other (permuter output-340-1; best.c holds the 78.2% one,
 * header-expanded). Symmetric spellings -- both inline, or both through the
 * same variable -- are what collapse to six instructions.
 *
 * WHAT IS ACTUALLY LEFT is register assignment inside that head, and nothing
 * else: every size-exact candidate still reports `first difference at +0xc`,
 * which is `adds r6, r1, #0`, the very first instruction after the frame. The
 * ROM puts a2 in r6 and the 0x8000 constant in r7; every candidate does it the
 * other way round, and the three head pseudos then land one register off all
 * the way down. Wave 17's rule applies -- of two values that tie on
 * allocno_compare the tie-break is the allocno NUMBER, assigned in RTL creation
 * order -- so the lever is whatever creates a2's reference BEFORE the 0x8000
 * constant's. Note the ROM copies a2 out of r1 before it has touched a1 at all,
 * which is a hint that a2 is referenced earlier in the original's statement
 * order than this two-call spelling can express.
 *
 * NEXT AGENT: do not re-run the permuter from best.c -- that basin is
 * exhausted. Attack the a2/0x8000 register pair directly, or find a statement
 * order that names a2 first. */
void sub_0803A2BC(u8 a1, struct Unit *a2)
{
  unsigned long long new_var;
  new_var = (a1 + 8u) >> 3;
  sub_08014A5C(new_var, 7, gUnknown_08499578, gUnknown_085D5ABC[a2->type].unk02, 0x8000, 0);
  sub_08014A5C(new_var, 0xd, gUnknown_08499578, gUnknown_085D5ABC[a2->type].unk04, 0x8000, 0);
}
