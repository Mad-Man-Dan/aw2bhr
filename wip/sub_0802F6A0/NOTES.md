## Wave 74 (W74-C)

Configured verification of the retained source is size-exact 604/604 with 327
different bytes (45.9% identical), first divergence `+0x0c`. Two clean,
chained `python tools/permute.py sub_0802F6A0 --current` runs (150 seconds then
90 seconds, three threads) drained without a match. The retained candidate
binds the wrapped cursor to a new `u16` in the same assignment that sets `t`;
it is semantics-preserving relative to the prior active draft and improves the
previous 337-byte residual by ten bytes.

Do not trust the current 49.8% `best.c`/`best.json`: the 49.7% and 49.8%
outputs initialize their replacement cursor/checksum locals after an
unconditional `return -4`, but use those locals from earlier `goto found`
paths. They are undefined-behavior candidates and were rejected after direct
inspection. The remaining credible mechanism is still the `dst`/`expSum`
hard-register versus stack-slot inversion described below; bounded lifetime
and allocation search did not resolve it.

/* Wave 50 (W50-D).  PARKED with a COMPILABLE DRAFT this time -- see
 * work/sub_0802F6A0/sub_0802F6A0.c.  Wave 49's read-out (kept below in
 * substance) was right about everything it claimed; the whole instruction
 * stream now reproduces, frame included (`sub sp,#0xc`, three slots).  The
 * residual is ONE CONTESTED CALLEE-SAVED REGISTER and nothing else.
 *
 * =========================================================================
 * THE TYPE MODEL, now settled by compiling it
 * =========================================================================
 *   s16 sub_0802F6A0(s8 slot, u16 *dst)
 *
 * - The three pool symbols are pool words, exactly as wave 49 said.  Write
 *   `gUnknown_03003128[slot]`, `gUnknown_03003F48[slot]` and
 *   `gUnknown_02025C18[cursor][slot]` -- all three are already declared
 *   volatile, and the ring is the TWO-DIMENSIONAL `volatile u16 [][4]`.
 *   NO NEW GLOBALS WERE NEEDED.  That part of the note was exactly right.
 * - `slot` is `s8`: every index use is `lsls #0x18; asrs #0x17` off one
 *   sign-extended value, and the u8 form spilled at [sp] is the parameter's
 *   own home slot.
 * - `len` is an `s16` LOCAL assigned from an `ldrh`.  That is what produces
 *   the ROM's split: the raw halfword sits in one pseudo (`mov ip, r0`) and
 *   every USE re-signs it (`lsls #16; asrs #16`), and the success return
 *   `return len * 2;` folds to the single `lsls #0x11; asrs #0x10`.
 * - `avail`, `sum`, `comp`, `expSum`, `expComp` and the wrap temp are all
 *   `u16`, and the two reads of `avail` are written `(s16)avail` -- the ROM
 *   truncates with `lsls #16; lsrs #16` and then sign-extends with
 *   `lsls #16; asrs #16`, two separate narrowings, which only a u16 variable
 *   plus an (s16) cast at the use gives.
 * - The available-length arms really are asymmetric, as wave 49 flagged:
 *     if (w < c) avail = w - (c + 0xFFFFFC00); else avail = w - c;
 *   the first arm evaluates c first and the second evaluates w first, and
 *   writing them in that order is what reproduces the load order.
 * - The payload loop is `i = 0; while (i < len) { i++; ... }` -- the `i++` is
 *   the FIRST statement of the body, which is why the multiplier is 1-based
 *   and the bottom test compares the already-incremented counter.  The ring
 *   element is written out THREE times inside the body (sum, complement,
 *   store) exactly as wave 49 predicted; hoisting it into a local loses three
 *   instructions per iteration.
 * - The marker scan is a `do { c++; c &= 0x3ff; if (marker == 0x4FFF &&
 *   c != w) goto found; } while (c != w); return -4;`.  The two `c != w`
 *   compares are separate in the ROM and separate in the source.
 * - The four failure returns are -2 (cursors equal at entry, and again when
 *   `len + 6 > avail`), -4 (fewer than two halfwords, scan exhausted, or
 *   `len > 0x80`) and -3 (sum/complement mismatch).  Wave 49 said they are not
 *   interchangeable and that is right -- note that -2 is used at TWO sites and
 *   -4 at THREE, all cross-jumped into one copy each.
 *
 * =========================================================================
 * THE EXACT REMAINING DIFF
 * =========================================================================
 * `dst` and `expSum` want the same register and agbcc gives it to the wrong
 * one:
 *   ROM:    `dst` gets NO hard register -- it is stored to its home slot
 *           [sp,#4] in the prologue and reloaded once into r5 in the loop
 *           preheader -- and `expSum` lives in sl for the whole tail.
 *   draft:  `dst` gets r8 (so `dst++` costs the extra `movs r0,#2; add r8,r0`
 *           instead of `adds r5,#2`) and `expSum` is spilled to [sp,#8].
 *           The draft also materialises the gUnknown_03003128 address once
 *           directly, giving one extra literal-pool word the ROM has not.
 * Net: 4 bytes over, and the instruction ORDER is already correct.
 *
 * RULED OUT (attempt 2, do not repeat): copying the parameter into a local
 * `q` before the loop and writing `*q = ...; q++`.  It does move `dst` to the
 * stack and does produce the ROM's `ldr r5,[sp,#4]` preheader reload, but it
 * makes things WORSE overall -- the frame grows to 0x10 and expSum still
 * spills, now to [sp,#0xc].
 *
 * NEXT: this is a decomp-permuter candidate on the wave brief's own criterion
 * -- a pure register-allocation residual on a >256-byte function with the
 * instruction order already right, which is exactly the case the brief says
 * the permuter is NOT useless on.  Do not hand-rewrite the statements; they
 * are correct.
 */

# Wave 92 (W92-A)

## The draft this wave inherited was not faithful, and its score was not real

The wave-74 draft scored 45.86% size-exact, and every wave since has quoted that
number. It was produced by adopting a permuter form whose `new_var` binding
changed what the code does: from the length read onward it indexed BOTH the
cursor table gUnknown_03003128 and the receive ring's player column by the
wrapped cursor value, and it also wrote `gUnknown_02025C18[t][t]`, using the
cursor as the player index as well. The original indexes both by `slot`.

Read off the ROM, twice, at two independent points:

  * at 0x0802F78E, `asrs r0,r2,#0x17` is (s8)slot * 2 and it is added to the
    cursor table's base, so the cursor is `gUnknown_03003128[slot]`;
  * at 0x0802F7B2, the same `asrs r4,r2,#0x17` is added to `cursor * 8` and to
    gUnknown_02025C18's base, so the ring element is
    `gUnknown_02025C18[cursor][slot]` -- 1024 rows of four halfwords, one per
    player, and `slot` selects the column.

`r2` there is the spilled `slot` byte reloaded from sp+0 and re-sign-extended,
which is why the same shift appears in every block.

The faithful draft is now at work/sub_0802F6A0/sub_0802F6A0.c and measures
604/604 (size-exact), 26.32%, first difference +0xc. THAT IS THE REAL BASELINE.
It is lower than 45.86% because the broken draft's wrapped-cursor indexing
happened to produce a register assignment closer to the original's; the score
was measuring the wrong program. `best.c` at 49.83% is the same kind of artefact
(wave 74 rejected it for reading locals that are assigned only after an
unconditional return) and must not be quoted either.

## The residual

Size-exact and the control flow is right. The difference is one allocation fact,
visible in the first hunk:

    ROM         str r1,[sp,#4]   -- dst spilled at entry, reloaded once
                                    (`ldr r5,[sp,#4]`) just before the copy loop
                mov sl,r3        -- the address word's address kept in sl and
                                    re-used at 0x0802F748 / F78E / F7FA
    candidate   mov r8,r1        -- dst kept in r8
                (no sl bind)     -- the address word rematerialised each block

Those are one fact, not two: the ROM carries one MORE long-lived value than the
candidate (the address-word pointer), which is what pushes `dst` out to the
stack. The stack frame is already the right size (12 bytes) and `slot` is at
sp+0 in both; only sp+4 and sp+8 are swapped (ROM: dst at +4, comp at +8;
candidate: comp at +4, dst in a register).

Already measured and not worth repeating: copying dst to a local before the loop
(dst reaches the stack but the frame grows and expSum still spills); widening
expSum from u16 to int (byte-neutral).

The permuter is the right tool from here -- size-exact, correct instruction
order, a pure allocation residual -- but it must be run from THIS draft, not
from best.c and not from the wave-74 draft.

## Two negatives on the wrap-around subtraction (measured this wave)

In the original, `avail` is built as `F48 - X` where X is the cursor, or the
cursor plus 0xFFFFFC00 when the writer has wrapped. The ROM builds the constant
from a POOL WORD (`.word 0xFFFFFC00`) and adds it to the cursor; our build
reassociates the whole expression and materialises 0x400 as `movs #128;
lsls #3`, then subtracts. Two ways of pinning the constant to the cursor were
tried and both are worse than the plain if/else:

  * binding it to an `int` local inside the taken arm
    (`wrapped = cursor + 0xFFFFFC00; avail = F48 - wrapped;`):
    608 bytes, +4, 19.41%.
  * a conditional expression with the subtraction outside
    (`avail = F48 - (F48 < cursor ? cursor + 0xFFFFFC00 : cursor);`), which
    is the shape the ROM's two arms and shared `subs r0,r0,r1` join look like:
    596 bytes, -8, 18.71%.

The plain if/else at 604/604 and 26.32% stands. Whatever pins the constant, it
is not a statement boundary and not the join.

## The permuter run was stopped on purpose, and why

One 900-second run was started from the faithful draft. It finished its search
and produced **854 output directories**, because the faithful draft scores low
enough that almost any mutation improves on it by permuter score.

`tools/permute.py`'s harvest has NO CAP: it verifies every output, spliced and
raw, which is up to 1,708 trymatch compiles -- hours of work. The run was
stopped during that phase, the process was confirmed gone, and the draft was
restored from `work/sub_0802F6A0/w92-faithful.c` and re-verified at 604/604,
26.32%.

THE DRAFT HAD IN FACT BEEN LEFT AS 204 KB OF HEADER-EXPANDED SOURCE at the
moment it was checked -- that is normal for the harvest (it writes each
candidate into the draft while trymatch judges it), but it means an unattended
harvest of this length leaves the function's deliverable unreadable for hours.
Anyone finding a 200 KB `.c` in this folder should restore `w92-faithful.c`
rather than investigate it.

The 854 outputs are kept under `work/sub_0802F6A0/permuter/`. A future wave can
verify the best of them cheaply without re-running the search -- the scores are
in each directory's `score.txt`, and the ones worth checking are the LOWEST
scores, not the highest.

FOR THE TOOLING: harvest should take the best N candidates rather than all of
them, or at least verify in score order and stop on a time budget. A search
started from a weak base currently produces a verification phase far longer
than the search it follows.

## wave 96

Base: `sub_0802F6A0.w96-start.c` (= faithful draft, 26.32%, size-exact). Now 61.2% at 608 bytes (+4), first difference +0xa (was +0xc).
Three source changes, each measured:
1. A separate payload pointer `u16 *q = dst;` walks the copy loop instead of `dst` itself. The ROM spills `dst` to [sp,#4] and loads it
   into a fresh register just before the loop (`ldr r5,[sp,#4]`); with `dst` as the walker it lived in r8 for the whole function.
   26.3% -> 30.1% (size -4). Two variables, not one: this is the wave-95 lever, and it transferred.
2. One shared `return -4` label placed at the avail test (`fail4:`), every other -4 return a `goto`. The ROM has ONE -4 block right after
   the avail test and every -4 branch goes there; the draft's return after the marker scan sat at the end of the loop. 30.1% -> 42.9%,
   size-exact.
3. `wrapped = cursor + 0xFFFFFC00; avail = write - wrapped;` as two statements. In one expression agbcc folds `w - (c + K)` into
   `(w - K) - c` and builds 1024 with `movs;lsls;adds`; the ROM keeps `c + 0xFFFFFC00` in a pool word and subtracts it. Statement split
   stops the fold. 42.9% -> 61.2% (size +4).
Residual: frame is `sub sp,#16` vs ROM #12 (q got a stack slot; the ROM keeps its walker in r5), `sum` in r9 vs r8, the ring base
(gUnknown_02025C18) sits in r8/ip where the ROM holds it in r6 via one direct pool load after the scan, and the ROM keeps the cell
address of gUnknown_03003128 in sl for the whole function. The un-binding suggestion in the prompt did not apply: the draft had no binds.
Proposed summary: does = pops one packet from a slot's receive ring; status = 61%, +4 bytes; left = walker register / frame slot,
ring base register; tried = walker pointer, shared -4 label, statement-split avail arithmetic.

### wave 96, permuter (two chained 600 s runs from the 61.2% file)
Run 1: 61.18% -> 92.05%, size-exact, first difference +0x1c. Run 2 (from the 61.35% cleaned base): -> 80.30%. BOTH are wrong C and were
rejected (kept as `permA-92-wrongc.c` and `permB-80-wrongc.c`): run 1 wrote `sum += ring[i = cursor][slot] * i;` and run 2
`i = ring[cursor][slot] * (++i); sum += i;`, i.e. the loop counter `i` is overwritten inside the loop (the counter then holds the cursor /
the product). Renaming the clobbered `i` to a fresh local (`cc = cursor`, or `prod`/`prod2`) drops both back to 61%. So the score comes
from `i` being one pseudo with extra sets, which changes what the allocator does with the counter; the true source has the ROM's product
in a separate register (`adds r1,r0,#0; muls r1,r2,r1; adds r0,r1,#0`) and the counter incremented AFTER the first ring load
(`adds r2,#1` sits between the load and the `muls`), so `* ++i` in the first product is the right spelling (61.18 -> 61.35%).
Adopted: the `* ++i` form (`vF96.c`), 608 bytes (+4), 61.35%.

## wave 97 (W97-V)
Base: levers 5b-258_1cp-259 (`sum += len + 0x4fff` through a block-scoped `u16 lv0 = len` copy stored into the loop counter `i`, which is then reset to 0). Value-preserving: `sum` is u16 so the s16-vs-u16 copy of `len` differs only by a multiple of 0x10000; wrongc OK. 61.35% +4 -> 81.29% size-exact. best.c (92%) is WRONG (wrongc: byte 0x3381 written 0xA2; it folds `i = table[..]` into the multiply). Round-2 levers: nothing above 81.29%. Permuter 540 s: no improvement. Residual at +0xa: frame is `sub sp, #16` vs ROM `#12` (one extra spill slot) and the zero constant sits in r9 where the ROM keeps it in r8.
