# Wave 84 notes: `sub_0805A0EC`

## W84-C

- Chained permute campaign replicating the W83-B win pattern (`--current`
  run then default-from-best.c run, -j4, 300 s each): BOTH runs were killed
  mid-search by an environment-level interrupt (KeyboardInterrupt/"Exiting."
  around iteration 13.7k/14.5k — not a clean client timeout; identical
  invocations completed normally for the sibling functions), but up to that
  point neither reported anything above the starting point.
- Combined with the two wave-84 pre-campaign aborted chains and the standing
  evidence (waves 62/66/70/77/80), the residual stands UNCONVERTED: 380/380,
  4 reported bytes of which ONLY the +0xd5/+0xd6 halfword pair is real
  (`movs r0,#0; strh r0,[r5,#2]` in ROM vs the same pair in r3);
  +0x34/+0x164 are relocation-addend bytes of the equivalent .rodata alias.
- `best.c` was audited: 380/380, first difference +0x34 — byte-equal to the
  active draft's binary, so it stayed unpromoted (unlike 59E3C's, which hid a
  genuinely better anchor).
- The W83-B mechanism did NOT reproduce: no dead-in-sequence copy re-ranked
  the tied allocno pair here. Park line: naked allocation tie, four parallel
  aborts now included in the negative evidence.

## W90-B -- still parked, PORT-EXACT at 98.9% (2 code bytes + 2 addend bytes)

Ported to PR #3 names (`struct Unk08499594` -> `struct Unit`; PORT-EXACT).
Two 900 s permuter runs with the fixed scorer:
1. undirected from the port: 9,368 iterations, nothing better.
2. directed, from the LITERAL-zero variant (w90-literal.c, 6 bytes),
   PERM_RANDOMIZE from `props = q + 0x12;` to `out->v = 0;`
   (w90-2.perm.txt): 9,699 iterations, best found only equals the port.

MECHANISM, read off `tools/rtldump.py --flags=-dg` (rtl-port/, rtl-lit/):
the two registers in play are RELOAD registers, and reload picks them
round-robin in insn order over the spill set {r0,r1,r2,r3} (see "A RELOAD
register is chosen ROUND-ROBIN" in docs/agbcc-codegen.md).
- Port: the key reload `ldr r2,[sp]` (insn 568) takes r2, the zero store's
  reload of the spilled `new_var3` (REG_EQUIV 0) takes r3 (577), then the
  rowp copy out of ip takes r2 (580; r0 is the insn's output and r1 holds p).
- Literal: the zero is a local-alloc pseudo in r0 (not a reload at all), so
  the ip copy is the next reload after the key's r2 and gets r3.
- ROM: zero in r0 AND ip copy in r2. As reloads that means the round-robin
  pointer sat one step later than in either candidate at that point: either
  one more counted reload between the key test and the zero store (port
  shape), or one fewer before the ip copy (literal shape -- e.g. if the key
  compare's `ldr r2,[sp]` were an ordinary insn, not a reload).
Tried and byte-neutral: copying `key` into a block-local just before the
compare (`k = key;` and `(k = key)` inline) -- cse propagates the copy away.
A from-scratch rewrite in the matched sibling's (c_0805A268.c sub_0805A388)
gMap-> member spelling is 356 bytes (-24), a different shape; not pursued.
The draft is restored to the port.

## Wave 91 (W91-B) -- member form, NEGATIVE

- `->height` / `->width` for the loop bounds, and `->terrain` / `->unit` /
  `(u8 *)->rowOffset` through the `q` cast: BYTE-IDENTICAL (98.95%).
- `rowp = (u8 *)&((struct Map *)q)->rowOffset[y]`: 95.79%. The member
  computes (y*2 + 0x417A) + q, and the ROM has (q + 0x417A) + t. The byte
  pointer is what gives the ROM's order.
- Dropping `q` for member access off the global (cast or gMap): 33.68%.
  The row address hoists into the outer loop.
- 900 s permuter from --current (35,635 iterations): no candidate better
  than the start.
- Temporary -O2 -fno-gcse profile: the configured-tuned draft goes -16.
Residual unchanged: the zero-store r0/r3 reload tie, 2 code bytes.

## Wave 92 (W92-C)

Unchanged: 380/380, two code bytes at +0xd5/+0xd6 (the store of zero uses r0 in
the original and r3 here). The two other reported bytes are the addend of the
equivalent read-only-data alias and are not part of the residual.

A 900-second permuter run from the current draft (12,235 attempts, four
threads) found nothing better.

The wave-90 description of the mechanism can be made one-directional, which
narrows the search. The compiler hands out scratch registers in a rotation,
one step per scratch it hands out, in instruction order:

- With the zero written as a plain literal it becomes an ordinary allocated
  value in r0, which is what the original has. The next scratch handed out
  after the one for the key test then goes to the row-pointer copy, and lands
  one register too high.
- So the original's rotation is one step **behind** this draft's at the row
  pointer copy, and the fix is one **fewer** scratch handed out somewhere
  **before the key test** — not an extra one after it. Everything before that
  point is already byte-exact, so the instruction that differs must be one the
  original writes without needing a scratch, most likely one of the two places
  where a value in a high register is read into a low one.

This is the same tie recorded since wave 70; the new part is the direction.

## Wave 94 (W94-B): fifth undirected run, nothing, and the fix cannot come from here

`--current`, 900 s, 4 threads, under the length-penalised scorer
(`AW2_PENALTY_SIZE=1000`) for the first time on this function. 34,891
iterations, 1,112 errors, **zero improving candidates** -- nothing beat the
starting point's objective score of 120, so nothing was verified and the draft
was not touched (re-checked afterwards: 380/380, 98.95%, first difference
+0x34, of which only the +0xd5/+0xd6 pair is code).

Runs from this draft now stand at five (waves 84, 86, 90, 91, 92 and this one,
counting only the ones that ran to their own deadline). The scorer fix changes
nothing here for a structural reason worth writing down: **the draft is already
size-exact, so the new length term is zero for it and for every size-exact
neighbour.** The fix removes a drift towards shorter candidates; it does not add
any pull towards a particular register assignment. On a park whose whole
residual is two bytes of register numbering, the objective is exactly as blind
as it was.

Read with the wave-92 direction note (the rotation must be one step BEHIND this
draft's at the row-pointer copy, so the change has to remove one scratch
hand-out BEFORE the key test, not add one after), this says the remaining work
is a reload-ordering read off the RTL dumps, not a search.

## wave 96

Base: unchanged draft (98.95% size+0, first +0x34; only +0xd5/+0xd6 real).
Hypothesis (move the zero local's first assignment, not its declaration) tested with the
one-unit harness: 8 placements of `new_var3 = 0;` (after rowp / cells / first guard / props /
third guard / after the volatile block / top / after off), read off the `.s`:
- after rowp, top, after off: zero in r3, ip copy in r2 (current draft; the trade's first half)
- after cells, first guard, third guard: zero in a hi reg copied down (`mov r2,sl` / `mov r2,r8`), ip copy r3
- after the volatile block (just before p bind): `movs r0,#0`, ip copy r3 (= literal; second half)
- after `props = q + 0x12`: zero HELD in sl and copied to r0 (`mov r0,sl; strh r0`) with ip copy in r2 -
  the ROM's two registers, but the zero is held in a hi register instead of `movs r0,#0`. try_match:
  21.58%, pool changes (an extra .rodata word appears, the 0816D980 alias is lost). Not a match.
So the live-range START does not compose the trade: the placements between give either a hi-reg
hold (more copies) or fall to one of the two known halves. Hypothesis refuted as a way to a match;
the two-way trade stands.
Proposed summary: does: lists candidate tiles for the active unit. status: 98.95%, 2 real code bytes.
left: zero store uses r3 instead of r0 (or the next row-pointer copy lands in r3). tried: zero local
placed at eight points, literal zero, dead extras, permuter runs (~50k iterations total).
