/* WAVE 86 (W86-G) -- READ THIS BEFORE THE W86-C BLOCK BELOW.
 *
 * THE ORCHESTRATOR'S PRE-REGISTERED QUESTION IS ANSWERED AND ITS HYPOTHESIS
 * IS REFUTED BY DIRECT READING OF THE ROM.
 *
 * Asked: "does the ROM's loop really carry BOTH strides as pointers, or is
 * one of `adds r4,#8` / `adds r2,#8` an offset the ROM re-adds to a base each
 * iteration (so the ROM has ONE pointer pseudo fewer than the draft, which is
 * exactly the frame difference)?"
 *
 * IT CARRIES BOTH, AND NEITHER IS RECOMPUTED. work/sub_08073228/target.s:
 *      lsls r4, r1, #3          k  = j * 8          (set up ONCE)
 *      ldr  r7, =gUnknown_08614024
 *      adds r2, r4, r7          p  = tbl + k        (set up ONCE)
 *  _0807327E:
 *      adds r0, r4, #0
 *      ldrb r7, [r2]            the compare reads through p, NOT through a
 *      cmp  r3, r7              recomputed r4+base
 *      beq  _08073294
 *      adds r4, #8              k += 8
 *      adds r2, #8              p += 8   <-- STEPPED, not re-derived
 *      adds r1, #1              j++
 *      b    _0807327E
 * `r2` is initialised from `r4 + base` exactly once, before the label, and is
 * thereafter stepped by 8. There is no per-iteration re-add of the base. THE
 * DRAFT'S THREE EXPLICIT STRIDES (j, k, p) ARE THE ROM'S THREE. Indexing
 * `tbl` by `k` with no pointer local would be a DIFFERENT loop from the ROM's,
 * so the suggested rewrite is not available and the frame difference is not
 * a pointer-pseudo count.
 *
 * SECOND HYPOTHESIS, MINE, ALSO REFUTED. The ROM's loop-body TOP block
 * computes i*0x100, i*2, i*0x10, acc<<16 and i+1 before the inner search,
 * which looked like source statements the draft was missing. Probed both
 * `int inext = i + 1;` at the body top with `i = inext;` at the bottom, and
 * that plus `int x10 = i * 0x10;`:
 *   - `inext` is BYTE-NEUTRAL IN SIZE and marginally worse (216 bytes, -4,
 *     169 of 216 differ, 21.4%, first difference +0xa -- against the plain
 *     draft's 168 of 216, 21.8%). gcc ALREADY emits `adds r2,r5,#1; mov r9,r2`
 *     at the body top and `mov r5,r9` at the bottom from a plain `i++`, so the
 *     ROM's `adds r3,r5,#1; mov sl,r3` IS NOT A SOURCE CONSTRUCT. Do not
 *     author a second counter here; the wave-58 reverse rule does not apply.
 *   - `x10` reorders two insns in the top block and changes the frame not at
 *     all. Frame stayed 0x1c in BOTH variants.
 *
 * WHAT THE 4 BYTES ACTUALLY ARE, counted off the diff (they are NOT the
 * frame -- `sub sp,#32` and `sub sp,#28` are the same two bytes, and the a1
 * spill is size-neutral: the ROM's three `ldr rX,[sp]` cost exactly what this
 * draft's three `mov rX,sl` cost):
 *   1. `mov r8, r7`   -- the ROM parks i*0x10 in a HIGH register, which costs
 *      a copy out of the low register the shift had to target.
 *   2. `ldr r0,[sp,#28]` -- the ROM spills i*2 instead and reloads it after
 *      the call. The draft makes the OPPOSITE choice on the same pair
 *      (i*2 in r6, i*0x10 spilled to [sp,#0x18]) and so pays neither.
 *   3. `adds r0, r4, #0` -- the ROM's dead copy of k at the top of the inner
 *      loop. W86-C measured that it cannot be authored (W80-F: a dead copy is
 *      deleted by flow, a live one coalesces) and that stands.
 *   4. the draft's own 2 bytes of `.short 0x0000` literal-pool alignment,
 *      which the ROM does not need because its code length is odd in
 *      halfwords.
 *
 * SO THE RESIDUAL IS A SPILL-CHOICE TIE BETWEEN TWO VALUES WITH IDENTICAL
 * PROFILES: i*2 and i*0x10 are each computed once in the body top and each
 * read exactly once after the call, in the same statement
 * (`proc->unk2a[i] = i * 0x10 + 4`, whose ADDRESS needs i*2 and whose VALUE
 * needs i*0x10). They are created in the same order in both. One goes to a
 * high register and one to the stack, and the ROM and the draft disagree
 * about which. This is the brief's "bare allocno tie with nothing behind it"
 * class. Nothing in the C text distinguishes the two operands of one store.
 *
 * NEXT LEVER, if a later wave wants one: it is a PERMUTER case, not a hand
 * case -- order-correct, slot-wrong, which wave 37 lists as the permuter's
 * own. No permuter run has ever been taken against the goto-loop draft; the
 * recorded negatives predate it. Chain from THIS draft. Do NOT go back to a
 * for/while loop -- that reopens the three facts W86-C settled. -- W86-G */

/* WAVE 86 (W86-C): THE INNER SEARCH LOOP IS A LABEL/GOTO LOOP -- CONFIRMED.
 *
 * The wave-46 park listed THREE facts as the remaining defect and predicted
 * they would move together. They did, and one construct fixed all three:
 * writing the inner search as a `goto` loop with EXPLICIT stride variables.
 * A goto loop emits no NOTE_INSN_LOOP_BEG, so loop.c never sees it -- no
 * rotation by `duplicate_loop_exit_test`, no LICM, no strength reduction
 * (docs/agbcc-codegen.md, "A GOTO LOOP IS INVISIBLE TO loop.c", W46-B).
 *
 * The loop body is now BYTE-EXACT against the ROM:
 *      ROM   ldrb r7,[r2] / cmp r3,r7 / beq / adds r4,#8 / adds r2,#8 /
 *            adds r1,#1 / b
 *      here  ldrb r7,[r0] / cmp r1,r7 / beq / add r4,#8 / add r0,#8 /
 *            add r3,#1 / b
 * -- unrotated, exit test at the TOP, unconditional `b` at the bottom, the
 * character on the LEFT of the compare, and all three strides as explicit
 * source variables. Every `for`/`while` spelling wave 46 tried rotates; this
 * one does not. THE THREE PARKED FACTS ARE ONE FACT AND IT IS SETTLED.
 *
 * THE NEW AND ONLY RESIDUAL: the frame is 0x1c, not 0x20. The explicit
 * pointer `p` that the goto loop requires (loop.c is not there to invent the
 * &tbl[j] giv) is one more pseudo, and the allocator pays for it by giving
 * `a1` the callee-saved sl instead of spilling it to [sp,#0] as the ROM does.
 * The ROM holds 5 cross-call values in registers and 2 on the stack (a1 and
 * i*2); this draft holds 6 in registers and 1 on the stack (i*0x10). It is
 * -4 bytes: the ROM's extra `ldr` to reload i*2, plus the dead
 * `adds r0,r4,#0`. Verbatim verdict, configured profile:
 *   size: candidate is 216 bytes, original is 220 (-4)
 *   bytes: 168 of 216 differ (21.8% identical), first difference at +0xa
 * The 21.8% is POSITIONAL and worse than wave 46's 29.1% only because the
 * divergence now starts at the frame instead of inside the loop. The
 * wave-46 draft is preserved verbatim at work/sub_08073228/w86-start.c.
 *
 * MEASURED THIS WAVE, do not repeat:
 *  - `m = k;` as the first statement of the goto loop, with `m` used after
 *    the loop for the .unk04 access: BYTE-IDENTICAL to not writing it. `m`
 *    coalesces with `k` and the copy is deleted. The ROM's dead
 *    `adds r0,r4,#0` CANNOT BE AUTHORED -- a dead copy is deleted by flow
 *    (W80-F) and a live one coalesces.
 *  - Moving `c = a1[i]` after the `k`/`p` init: reorders those three
 *    instructions and changes nothing else. Frame still 0x1c.
 *
 * NEXT LEVER: the frame only. Something must cost one more cross-call
 * register so `a1` spills to [sp,#0] and i*2 to [sp,#0x1c]. Do NOT go back
 * to a for/while loop -- that reopens all three settled facts.
 *
 * WHAT IT DOES: for each character of the string at a1, linear-search the
 * 8-byte-record table gUnknown_08614024 for a record whose `unk00` equals it,
 * CpuFastSet that glyph's 0x100 bytes from a2 into VRAM at
 * 0x06010000 + a3 * 0x20 + i * 0x100, then record the glyph's OAM x
 * (`proc->unk2a[i] = i * 0x10 + 4`) and its running pen position
 * (`proc->unk46[i]`), advancing the pen by the record's `unk04`. Finally
 * proc->unk58 = the character count and proc->unk60 = count * 4 + 0x14.
 *
 * SETTLED IN WAVE 46, still true and still required:
 *  - `short acc` -- a SIGNED short LOCAL -- is what produces the ROM's
 *    `lsls #16; asrs #16` on read and `lsls #16; lsrs #16` on write.
 *  - The PARAMETERS USED DIRECTLY with a cast at each use, never through
 *    aliasing locals `str`/`proc`.
 *  - [sp,#0x10], [sp,#0x14], [sp,#0x18], [sp,#0x1c] are compiler temporaries
 *    and must NOT be authored as source locals.
 *  - The prototype. Toolchain (no override anywhere in 0x08073).
 *  - `gUnknown_08614024 + 4` vs asm/'s `gUnknown_08614028` is the documented
 *    same-address false mismatch, not a difference. */

#include "global.h"

#include "proc.h"

struct Unk73228Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a[(0x46 - 0x2a) / 2];
    /* 0x46 */ u8 unk46[0x58 - 0x46];
    /* 0x58 */ int unk58;
    /* 0x5c */ u8 filler_5c[0x60 - 0x5c];
    /* 0x60 */ u16 unk60;
};

/* WAVE 87 (W87-B) -- THE BODY BELOW IS A PERMUTER OUTPUT. DO NOT TIDY IT.
 *
 * State, configured profile: 220/220 SIZE-EXACT, 76.4%, up from the wave-86
 * goto-loop draft's 216 bytes (-4) / 21.4%.  Two chained permuter runs, the
 * first ever taken against the goto-loop draft (every recorded permuter
 * negative in this park predates it and is therefore void).  See W87-notes.md.
 *
 * The construct that did most of it: `int c = ((const u8 *)a1)[i];` is GONE and
 * the compare writes `((const u8 *)a1)[i] == *p` out again.  UN-binding that
 * local is what bought the ROM's whole prologue -- the 0x20 frame and
 * `str r0,[sp] / str r1,[sp,#4] / str r3,[sp,#8]`, the parameters spilled to
 * their stack homes in parameter order -- which four waves had read as register
 * pressure.  `int c;` is left declared and unused and `j = j * 8; k = j;` is
 * the permuter's spelling of `k = j * 8;` (j is 0 there); both kept verbatim.
 *
 * The wave-86 draft is preserved as _w87_gotoloop.c and w86g-start.c.
 */

extern const u8 gUnknown_08614028[];

void sub_08073228(const void *a1, void *a2, u16 a3, ProcPtr a4)
{
  struct Unk73228Proc *new_var3;
  int k;
  int i;
  int j;
  const u8 *p;
  short acc;
  const struct Unk08614024 *new_var2;
  int new_var;
  int c;
  const u8 *glyph;
  j = 0;
  if (1)
  {
    i = 0;
    acc = 4;
  }
  while (((const u8 *) a1)[i] != 0)
  {
    j = j * 8;
    new_var = 8;
    k = j;
    p = ((const u8 *) gUnknown_08614024) + k;
    new_var3 = (struct Unk73228Proc *) a4;
    new_var2 = (const struct Unk08614024 *) gUnknown_08614028;
    _search:
    if (((const u8 *) a1)[i] == (*p))
    {
      goto _found;
    }

    k += new_var;
    p += new_var;
    j++;
    goto _search;
    _found:
    glyph = ((const u8 *) a2) + (j * 0x100);

    CpuFastSet(glyph, (void *) ((j = 0x06010000 + (i * 0x100)) + (a3 * 0x20)), 0x40);
    new_var3->unk2a[i] = (i * 0x10) + 4;
    ((struct Unk73228Proc *) a4)->unk46[i] = acc;
    acc = acc + (*((int *) (((u8 *) new_var2) + k)));
    i++;
    j = 0;
  }

  ((struct Unk73228Proc *) a4)->unk58 = i;
  ((struct Unk73228Proc *) a4)->unk60 = (i * 4) + 0x14;
}
