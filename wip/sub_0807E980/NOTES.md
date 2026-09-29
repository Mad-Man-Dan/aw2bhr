# sub_0807E980 notes

## W84-A (wave 84, agent A)

Draft state at wave start: champion old-shape draft restored before this agent
(`sub_0807E980.c` == w83-snapshot.c == w83-best-before.c == best.c,
MD5 ED91F14A..., 99.04% / 10 bytes / first diff +0x370). The "DRAFT GONE"
preflight flag was stale.

**The staged lead WAS measured now (twice), both under configured profile.**

1. `w84-comma-depth.c` -- wave 83's exact spelling: comma binding
   `base = (int)&gUnknown_0200FC50` inside the FIRST `sub_08043BA4` third
   argument, loop base `&((u8 *)base)[(i*0x100)+(j*0x400)]`. try_match: MISS.
   The base pseudo materializes ~4 instructions too early
   (`ldr r3,=gU; mov sl,r3` lands BEFORE the first sub_08043BA4 where ROM has
   `ldr r4,=gU` only after the second call), shifts everything +4 bytes, flips
   the .rodata pool order (candidate 0200FC50@400/08234B10@404 vs ROM
   08234B10@400/0200FC50@404) and swaps r4<->r5 through the whole trailing
   sprite loop.

2. `w84-decomp-comma.c` -- correction probe: same comma moved INTO
   Decompress's second argument
   (`Decompress(gUnknown_08234B10, (base = (int)&gUnknown_0200FC50,
   (void *)base))`). Isolated compile_probe shows correct unification and
   pool order for the loop itself; full-function try_match: MISS. Same
   defect class: because an explicit int local crosses `bl Decompress`,
   reload assigns its callee-saved home AT THE ASSIGNMENT (`mov sl,r3`
   immediately after `ldr r3,=gU`, before the call), while the ROM keeps the
   value in scratch r4 through the call and only homes it to sl via the late
   copy sequence `movs r5,#0; mov sl,r4`. Plus the same global allocno
   reshuffle (r4<->r5 in both sprite loops).

Conclusions:
- The comma MECHANISM is confirmed: with any explicit binding, one pseudo
  carries &gUnknown_0200FC50 into the loop with no per-iteration pool reload --
  the thing three waves asked for does happen.
- But every explicit-binding anchor that was authored creates the pseudo as a
  USER LOCAL whose home is chosen at the assignment; the ROM shape has NO user
  variable there at all -- CSE unifies the two references invisibly and only
  adopts sl at the loop preheader. No placement of an explicit binding can
  reproduce that home-timing; this axis is CLOSED for spellings of the form
  measured (anchor = first sub_08043BA4 third arg, anchor = Decompress second
  arg).
- Draft restored to the champion (verified by hash). best.json untouched
  (99.04). Park entry stands; nothing new beyond the two measured numbers
  above to add to it.

## W90-A (wave 90, agent A) -- 99.0% -> 99.4%, 10 -> 6 bytes, size-exact

Active draft = `w90-R6-994.c` (configured: 1040/1040, 6 of 1040 differ, first
difference +0x370). `w90-start.c` is the 99.0% champion this wave began from.

**The inner loop is a WALKER + GIV pair, not two givs of j.** New source:

    for (j = 0, new_var = 0; j < 8; j++)
    {
        x = (0x06015000 + (i * 0x800)) + new_var;
        CpuFastSet(&gUnknown_0200FC50[(i * 0x100) + (j * 0x400)], (void *) x, 0x40);
        new_var += 0x100;
    }

- `new_var` is a source walker (biv, step 0x100) and the VRAM address is a
  GIV OF THE WALKER, not of j. strength_reduce then inserts the dst increment
  before the walker's own (body) increment, i.e. BEFORE the src giv's increment
  (which goes before `j++`). That reproduces the ROM's dst-then-src loop bottom,
  which no two-givs-of-j spelling can (both givs of one biv always increment in
  giv-list order, the same order as their inits). R1 (the walker sum inline in
  the call) = 8 bytes; R6 (the sum in its own statement BEFORE the call) = 6
  bytes, because computing `i * 0x800` first puts `lsls r1,r5,#11` ahead of
  `lsls r0,r5,#8` as in the ROM.
- The ONLY residual left: `movs r6,#7` (counter init) sits after the dst init
  where the ROM has it between src and dst. RTL dumps (`-dL`) settle where
  each preheader insn comes from: `i + 1` and `i << 8` are GCSE/PRE insertions
  (present in the `.gcse` dump) at the end of the block before the inner loop;
  the src init is a RUN-1 giv init; the counter `j = 7` is written by
  check_dbra_loop in the SECOND loop pass (`flag_rerun_loop_opt`), and
  check_dbra_loop runs BEFORE that pass's own giv reduction. So in the ROM the
  dst init, emitted after `j = 7`, must be a giv reduced in RUN 2, and its
  increment still precedes src's, so its biv is not j. In every spelling
  measured here dst is reduced in run 1. That is the open question, and it is
  now narrow: what makes the walker's giv "not worth while" (or not a giv) in
  pass 1 but reduced in pass 2? LICM's threshold test (13 x savings x life vs
  insn_count 19 -> 16) has no integer landing between the two passes for a
  life-1 movable, so it is not the obvious one.

Measured, all configured, all worse or equal (drafts in the scratch variants,
not kept): the matched twin `c_080790D0.c`'s full walker form (src/dst walkers,
`k = i * 0x800`, descending source `j = 7`) = +12, 54.1%: the three extra user
pseudos push `proc` out of r7 (r9) and `p = gUnknown_0200FC50` is
rematerialised from the pool every outer iteration instead of held in sl; with
p / without p / int walkers / `off` walker all identical or worse. dst walker
only, src giv of j (`I`) = 96.5% but dst init is SOURCE (first in the
preheader); walker counting by 1 (`* 0x100` at the use) = R1; walker in the
for-increment = 98.0%; do/while(0) around any one of the three body statements
or the whole body = byte-identical to R6 (jump deletes the zero-trip loop
before loop opt, so it cannot defer a reduction to pass 2); src as a source
walker with dst a giv of the walker (descending or ascending j) = 85.7%.

Permuter: directed run from the old 99.0% base (96 PERM_GENERAL bases x
randomize over the loop, 9,242 iterations, 900 s) found nothing; the walker
form was found by hand after reading the RTL dumps.

**Tooling added this wave: RTL dumps.** `agbcc` accepts gcc 2.95's `-d`
flags. Preprocess with the same cpp line as `permuter/compile.sh` into a file
and run `agbcc x.i ... -O2 -dr -dL -dG -dc -o x.s` in a scratch dir: you get
`x.i.rtl` (expand), `x.i.gcse`, `x.i.loop` (with the per-loop biv/giv log for
BOTH loop passes: "giv ... reduced to", "Can reverse loop", "not worth
while"), `x.i.combine`. This answers "which pass wrote this insn" directly.

**Run 3 (wave 90, W90-A):** undirected 900 s from the 99.4% R6 draft
(`w90-R6-994.c`, base score 60), 13,173 iterations: nothing better. Totals on
this function this wave: three runs, ~36.8k iterations, no permuter gain; the
10 -> 6 byte step was the hand-found walker form. The directed sources are kept
as `w90-directed.perm.txt` and `w90-R6-directed.perm.txt`. Draft left at 99.4%
(`sub_0807E980.c` == `w90-R6-994.c`).

## Wave 92 (W92-C)

Current draft: 1040/1040, 6 bytes differ (99.4%), first difference at +0x370,
in the set-up of the copy loop at the tail.

A 900-second permuter run from the current draft (12,235 attempts) found
nothing, which repeats the wave-90 result from the same starting point.

New this wave: `work/sub_0807E980/w92-ptr.c` holds the pointer form the wave-81
notes describe — source and destination as plain `int` locals stepped in the
loop body, with the three set-up statements written in the original's own order
(source, then the counter, then the destination). It scores 53.5% and is 12
bytes too long, matching the earlier measurement. Its value is that its
instruction order inside the loop is the original's and only the register
choices are wrong, which is the case the automatic search is meant for; every
earlier search started from the high-scoring draft, whose loop order the search
cannot reach. A run from this base is the first of its kind.

### What the pointer form actually costs, measured

The earlier notes say the pointer form is 12 bytes too long because it adds two
pointer locals. That is wrong. Writing the same do-while loop using only
locals the function already declares — no new declarations at all — is still
12 bytes too long (52.9% identical). The cost is not the number of names.

Reading the prologue diff gives the real cause in one line: with the do-while
loop the screen's record pointer lands in r9, where the original keeps it in
r7 (`mov r9, r0` against `adds r7, r0, #0`). The copy loop's own instructions
are all present and correct; only their registers differ. That one placement
cascades through the whole function and is the entire 12 bytes.

Two attempts to force it, both measured:

- Asking for the three loop values in the registers the original uses
  (`register int x asm("r4")` and so on for the source, the destination and
  the counter) is **byte-identical** to not asking — three separate variants,
  all 12 bytes long and 53% identical. The request is inert here.
- Asking for the record pointer in r7, by renaming the parameter and copying it
  into a pinned local, is **much worse**: 1032 bytes (8 short) and 18.6%
  identical. The whole function is rebuilt around the pin.

So the open question is unchanged but better stated: the do-while loop needs one
more callee-saved register than the original's shape does, and the register it
takes is the one holding the record pointer. Asking for registers by name does
not recover it.

### The automatic search does move this base

A 900-second run from the pointer form climbed it from 53.5% to 94.4%, ending
size-exact with its first difference at +0xff. That result is kept as
`work/sub_0807E980/w92-ptr-perm1.c`. It is still below the active draft, so the
draft is unchanged, but it is the first time this starting point has been
searched at all, and three separate 900-second runs from the 99.4% draft (two
in wave 90, one here) have now produced nothing. Chaining further runs from
`w92-ptr-perm1.c` is the better use of the next search budget.

## Wave 93 (W93-F): the wave-92 starting point is wrong C — withdrawn

Wave 92 ended by recommending `w92-ptr-perm1.c` (94.42%, size-exact) as the
base for the next search, on the grounds that it was the first result from a
starting point whose loop order is the original's. **That file does not do
what the function does**, and the recommendation is withdrawn. It is now
`w92-ptr-perm1.c.wrongc`.

Its copy loop reads:

    vram_p = i;
    do { CpuFastSet(src, (void *)(0x06015000 + vram_p * 0x800), 0x40);
         vram_p += 0x100; ... } while (j >= 0);

The destination is scaled by 0x800 *after* the step is added, so it advances
0x80000 per pass. The function copies eight 0x100-byte rows into consecutive
VRAM, 0x100 apart. Seven of the eight destinations are wrong.

**Why it scored 94.42% anyway**, which is the part worth keeping: the
compiler strength-reduces the scaled destination into a stepping pointer
either way, and the step is materialised as a two-instruction constant. The
ROM's 0x100 is `movs r0, #0x80; lsls r0, #1`; the broken 0x80000 is
`movs r0, #0x80; lsls r0, #0xc`. Same instructions, same length, one
immediate field apart. A wrong constant in a strength-reduced loop is almost
free in the byte score, so the byte score cannot see this class of error at
all. Nothing about the scoring was at fault — it measured what it measures.

Corrected to the ROM's semantics, keeping everything else the search found
(`w93f-ptr-fixed.c`: `vram_p = 0x06015000 + i * 0x800`, passed to CpuFastSet
directly), it measures **51.24% at +12 bytes** — the same +12 as `w92-ptr.c`
before any search. So the entire 53.5% -> 94.4% climb was the broken loop.
The pointer form's real cost is unchanged and the open question is still the
one wave 92 stated: the do-while shape needs one more callee-saved register
than the original's, and it takes the one holding the record pointer.

Draft restored to the 99.42% form (`sub_0807E980.w93-start.c`), unchanged.

## Wave 94 (W94-B): fourth undirected run from the 99.42% draft, nothing

`--current`, 900 s, 4 threads, under the length-penalised scorer
(`AW2_PENALTY_SIZE=1000`) for the first time. 30,055 iterations, 944 errors,
**zero improving candidates reported** -- the search never beat the starting
point's objective score of 60, so there was nothing to verify and the draft was
not touched.

This is the fourth run from this draft (two in wave 90, one in wave 92, this
one) and the first under the fixed objective. The objective was the reason
wave 93 gave for retrying this function; the retry is negative, and on this
draft the fix could not have helped anyway: the draft is already size-exact,
so the length term is zero for it and for every size-exact neighbour, leaving
the ranking exactly as it was.

The residual is unchanged and is still the one wave 90 stated precisely: the
inner copy loop's counter init `movs r6,#7` sits after the destination init
where the ROM has it between the source and destination inits, because in the
ROM the destination pointer is a giv reduced in the SECOND loop pass (after
check_dbra_loop wrote the counter) and in every spelling measured here it is
reduced in the first. That is a question about `loop.c`'s reduction threshold
across the two passes, not about register allocation, which is why an
allocation search cannot reach it.

## wave 97

Base: the 99.42% draft (`sub_0807E980.w97-start.c`), restored as the final source. It is unchanged.

New finding on the residual (the `movs r6,#7` position): making the SOURCE pointer a walking local
(`u8 *src = &gUnknown_0200FC50[i * 0x100]; ... src += 0x400;` inside the `for (j...)` body, declared in a block around the loop,
with the destination left as the `(0x06015000 + i*0x800) + new_var` giv) moves `movs r6,#7` to the ROM's place:
after the source init and BEFORE the pool load of 0x06015000 and the destination sum. Reason: the source is then an
ordinary biv whose init is an original insn in source order, the loop counter's `j = 0` (rewritten to 7 by check_dbra_loop)
follows it, and only the destination giv init is emitted afterwards at loop start. This CONFIRMS the wave-90 reading that the
order is the ROM's giv/biv split, and shows the lever is which of the two pointers is a biv. The variant
(`sub_0807E980.q1.c`, increments in the order `new_var += 0x100; src += 0x400;`, which matches the ROM's step order) is
size-exact but scores 97.21% (first diff +0x364): the two increment constants take r2/r3 instead of r0/r1, and the
preheader is ordered `lsls r0,#8; add r4; add r0,r5,#1; mov r8; lsls r0,#0xb; movs r6,#7; ldr; adds` where the ROM has
`adds r3,r5,#1; mov r8,r3; lsls r1,#0xb; lsls r0,#8; mov r2,sl; adds r4; movs r6,#7; ldr; adds`. So the trade is now
between ONE misplaced constant and a shuffled preheader plus two register numbers. Making both pointers walkers (w1-w4 in
build/probe/w97j.py) costs 12 bytes (an extra register). A 900 s permuter chain from q1 (2 threads): NO-IMPROVEMENT: the text score fell from 2160 to 1280, but every candidate that was verified scored below q1 (best 93.1%, others 80-92%), so nothing was adopted.
Pre-registration (this batch): none for this function. Proposed summary: unchanged 99.42%; add to `tried`: source pointer as a
walking local (moves the counter init to the ROM's place but shuffles the preheader and constant registers, 97.2%).

## wave 97 (second pass)

Base: `sub_0807E980.q1.c` (source pointer as walker, 97.21%). Draft `sub_0807E980.c` unchanged (99.42%).
Read the RTL (-da, .greg): the wrong constant registers are RELOAD hand-outs, not allocation. `r5 += 0x100` and `r4 += 0x400` need a register operand, so reload creates insns 1399/1402 and takes spill regs round-robin: the 99.42% draft gets r0, r1 (ROM), q1 gets r2, r3 because the hand-outs earlier in the preheader (`mov r3,sl` for the base) already advanced the rotation. So the constant registers follow the preheader ORDER, and the preheader order is the thing to fix. ROM order: [i+1 copy][i<<11][i<<8][base+ (mov r2,sl)][movs r6,#7][pool 0x06015000 + add]. q1 order: [i<<8][mov r3,sl; add][i+1][i<<11][movs r6,#7][pool][add] - the source init is an ordinary insn in source position and precedes the hoisted invariants.
Tried making the `i+1` and `i*0x800` come first in source (copy-back outer loop `for (i = 0; i < 4; ) { int ni = i + 1; ...; i = ni; }`, with and without `int d = i * 0x800`, with the base bound to `u8 *base`): the counter init sits in the ROM's place and the constants get r0/r1, BUT global allocation changes (proc moves to r8, `ni` takes r7, the base is no longer held in sl, +8 bytes, 54%). Swapping the two increments' order, `j = 0, new_var = 0` order: 97.21% unchanged.
The sibling sub_0807F434 has the same loop; the copy-back outer loop worked there (see its NOTES), so the difference is register pressure in this bigger function (proc, base and ni compete for r7/r8/sl).
Untried: copy-back outer loop plus something that lowers `ni`'s weight below proc's (e.g. compute `ni` after the inner loop from `i`, which is what the draft already does).

## wave 97 (W97-PG)
Permuter chain: 1 link (540s), 99.42% -> 99.42%, NO-IMPROVEMENT. Draft unchanged.
