
## wave 95

Base: the existing draft (`sub_080546F0.w95-start.c`, unchanged, 26.6%, +8). Not improved.
Only hand probes, no permuter (not size-exact; permuter reserved for the size-exact drafts).

- Counter TYPES are not the seed: `u8 i` -864 (-196), `u8 i,j` -228, `int i; u8 j` +12, `s16 i` +32, `u32 i,j` +8 (same as int), `int i; s16 j` +20.
- The frame difference is confirmed as pure allocation: draft and ROM are the same code up to the first
  loop, but the draft puts `i` in r6 and the 0x030045A0 pointer in r7, the ROM the reverse (i in r7, pointer r6).
- `gUnknown_02029664 = 0` as its own byte zero (ROM: `movs r0,#0; strb r0,[r1]`, a second zero): a
  `volatile u8` store and a `u8 tmp8 = 0` temp are both byte-neutral (+8, 26.6%). The byte zero is not
  authorable by a temp: cse still merges it with the halfword zero, or the statement is unchanged.
- One distinct temp per block (W95-A lever): does not apply. There is no reused compare/bound variable here;
  the shared object is the loop counter itself, which the wave-72 split table already measured. Transfer: NO.

Proposed summary: status +8 bytes, 26.6%; left unchanged (the ROM keeps the shared counter in r7 and
the sl/sb/r8 pointer set differs); tried gains: counter types (six spellings), byte-zero temp / volatile store.

## wave 97 (W97-G)

Not worked beyond re-reading the diff (budget spent on earlier functions). Observed: the first difference is the
final `gUnknown_02029664 = 0` byte store, where the ROM loads the address into r1 and makes a fresh `movs r0,#0`
(the halfword-zero register was dead by then), and the frame is 84 vs ROM 72. No probes run; draft unchanged (26.59%, +8).

## wave 97 (W97-Q)
Base: `sub_080546F0.w97q-perm1-start.c` (= existing draft, 26.59%, +8). Read the whole diff: spill traffic differs mostly in loop 3 (30+ array stores). The ROM stores 8 loop-invariant bases to `[sp,#28..#64]` and reloads each once per array group; the draft keeps its own set at `[sp,#44..#64]` and additionally spills/reloads the same slots INSIDE the loop body (`ldr rX,[sp,#N]; str rX,[sp,#N]` pairs at the end of the body, none in the ROM). Frame 84 vs 72 = three more slots. One 900 s permuter run from the base, 2 threads (was the only new probe): see below.

Result (W97-Q): 26.59% +8 -> 44.25% SIZE-EXACT (1060), first diff +0x23, frame now 0x48 = ROM. Six chained permuter links (26.59 -> 41.70 -> 41.89 -> 42.26(hand) -> 42.36 -> 43.40 -> 44.15 -> 44.25; two further links NO-IMPROVEMENT); each kept file passed `wrongc.py` (OK, 48-52 seeds, 81% of code reached). Final source is `sub_080546F0.c` (`new_var` renamed `zero`, byte-neutral; raw permuter file `sub_080546F0.w97q-final-raw.c`).
What moved it: ONE shared zero local (`zero = 0` chained into the `gUnknown_02029664 = (zero = 0)` store) used as the source of about half of the zero stores, while the other zero stores keep the literal `0`. This splits the compiler's single zero constant into two live values, as the ROM has (a halfword zero and a separate byte zero), which removes the three extra spill slots and puts the loop counter in r7. Which stores use `zero` matters and is permuter-chosen, and `for (i = zero; ...)` in loops 1 and the fill loop. A hand-move of the `zero = 0` assignment from `gUnknown_030045B0` to `gUnknown_02029664` gained +0.4.
Also kept: `q = gUnknown_02029668[i]; ((struct VRow5 *)q)->v[j] |= 0xffff;` (the row pointer bound to the existing local q).
Residual: register naming in loops 1-3 (which array base sits in which hi register) and the order of the pointer setup; 591 of 1060 bytes differ. Next: walk the first hunk from +0x23 by hand; the choice of which zero stores read `zero` is a free axis worth a directed PERM_RANDOMIZE.
Proposed summary: does = resets the tables for two players (counters, unit rows, palettes/tiles fill via CpuFastSet); status = 44% at exact size, frame matches; left = which base pointers live in which registers in the three init loops; tried = counter types, byte-zero temp, shared-zero local (the lever), split counters.
