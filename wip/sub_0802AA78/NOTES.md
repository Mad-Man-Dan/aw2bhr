# sub_0802AA78 — Wave 74 evidence

Status: parked, not matched. The final configured `try_match` verdict is false.

## Strongest independently verified draft

- Source: `sub_0802AA78.c`
- Target size: 2356 bytes
- Candidate size: 2356 bytes
- Differing bytes: 253
- Identity: 89.2615% (matcher display: 89.3%)
- First matcher-visible difference: +0x90
- Last differing byte: +0x8A8
- First non-relocation instruction region: target/candidate instructions split at +0x398; the first differing instruction byte is +0x399.
- The configured matcher did not declare the forced-address pool relocations equivalent, so they are not accepted as a match.

The retained compiler-shaped levers are:

1. Only the `sub_0802BB74` arm uses `(cx + tbl) + gUnknown_0849A284[6]`; the `sub_0802BAFC` arm keeps `gUnknown_0849A284[6] + (cx + tbl)` to avoid GCSE shrinking the routine.
2. The third linked-unit branch binds `unit->unk07` through the existing `t` temporary before indexing `gUnknown_08499594`.
3. A function-scope zero temporary is defined after the second window-bound calculation and reused in `accv != zero`. This is semantically identical to `accv != 0` and changes allocation/lifetime; it improves 254 differences to 253 without changing size.

The canonical local `struct Map` and member spelling `Tbl49A2A6` were left unchanged.

## Honest baseline and stale artifact

The preserved Wave 74 starting source is `wave74-start.c`, SHA-256 `3C987FC249B1ADEFB4888033F9E03D9FF30C2582D4CBEFAAD98421628A8D5BEB`.

- Configured baseline: 2356/2356 bytes, 286 differing bytes, 87.8608% identity (matcher display: 87.9%), first difference +0x90.
- The previously recorded 92.53% / 176-byte claim was not reproducible from the include-based active draft.
- `best.c`/`best.json` are header-expanded/stale artifacts. During the clean continuation, an expanded-header form measured 2356/2356 and 144 differing bytes (93.9%), while its actual clean candidate and normal include-based reconstruction independently measured 2352 bytes and 426 differing bytes. The 93.9% artifact was rejected and was never used as a seed or adopted into the active draft.

## One-axis results

Pool-order / GCSE axis:

- Original association in both arms: 2356 bytes, 286 differences (87.9%).
- Reassociate only the `sub_0802BB74` arm: 2356 bytes, 255 differences (89.2%).
- Reassociate only the `sub_0802BAFC` arm: 2356 bytes, 259 differences (89.0%).
- Reassociate both arms: 2352 bytes, 427 differences (81.7%).
- Bind `cx + tbl` locally in both arms: 2360 bytes, 1344 differences (43.0%).
- Reassociated first arm / local second arm: 2356 bytes, 909 differences (61.4%).
- Local first arm / reassociated second arm: 2356 bytes, 972 differences (58.7%).

`q` merge axis:

- Function-scope `int qr` assigned into function-scope `s16 q`: same as the 286-difference baseline.
- Explicit `(s16)qr`, inner-scope `s16 q = qr`, and `qr += 0` lifetime spellings: same as baseline.
- Function-scope `int q` plus signed casts: same as baseline.
- `u16 q` plus signed casts: 2360 bytes, 554 differences (76.5%).
- Thus the earlier note that `int qr` plus `s16 q` emitted the desired `lsls`/`asrs` pair is not reproducible in the current include-based translation unit.

Allocation/permuter axis:

- Clean run 1: `python tools/permute.py sub_0802AA78 --seconds 300 --threads 3 --current`; base permuter score 1445, over 5500 iterations, no match. The normal-source `t = unit->unk07` result independently measured 2356 bytes and 254 differences.
- Clean run 2 from that verified source: same command; base score 1440, over 5900 iterations, no match. The normal-source zero-temporary result independently measured 2356 bytes and 253 differences and is retained.
- The run's apparent score-650 / 93.9% path was the expanded-header contamination described above. It was independently rejected.

## Remaining mismatch shape

- Forced-address pool relocation bytes start at +0x90 and recur before the first instruction mismatch. They remain part of the configured false verdict.
- In the +6 if/else pair, the target loads `Tbl49A2A6` before `gUnknown_0849A284` in both arms. Fixing both arms exposes GCSE and loses four code bytes; fixing one arm preserves exact total size and is the strongest tested tradeoff.
- At the final `q` merge, the target emits `lsls`/`asrs` signed materialization and reuses that value. The include-based candidate proves the `u8` range and rotates registers instead; tested declaration, cast, scope, and lifetime variants did not reproduce the pair.

The requested guide filenames `docs/decomp-principles-and-practice.md`, `docs/dual_toolchain_matching.md`, and `docs/decomp-matching-discoveries.md` are absent from this checkout. The available shared brief and `docs/agbcc-codegen.md` guidance were used instead, along with promoted vocabulary exemplars `src/decomp/c_0802AA14.c` and `src/decomp/c_0802B4D4.c`.

## Wave 93 (W93-D) -- 89.26% -> 98.05%, size-exact, by five chained permuter runs

The park said two clean chained runs over 11,000 iterations had found nothing.
That was true of the run, not of the method: five runs chained from each
other's kept improvement moved it 89.26 -> 93.80 -> 95.84 -> 97.28 -> 98.05,
and the sixth found nothing. Every gain is a BINDING LOCAL or a
REASSOCIATION -- no statement was added, removed or reordered:

  * the first sub_0802BAFC argument in the hp branch bound to `q` (which is
    dead there; `q = 0` is assigned later, before its own first read, so the
    reuse is a dead-range reuse and not a semantic change);
  * `gUnknown_0849A284[1]` indexed through a local holding 1 (now `yIndex`);
  * the table's unk04 column base bound to a local (now `tblUnk04`);
  * `accv != zero` written as `accv != 0` (`zero = 0` is assigned earlier, so
    the two are the same test);
  * three address sums reassociated, e.g.
    `gUnknown_0849A284[6] + (cx + tbl)` to `(cx + tbl) + gUnknown_0849A284[6]`.

Every changed statement was read and checked; `drafts.py bases` reports no
read-before-set and names this file as the base. The two permuter temporaries
were renamed from new_var/new_var2 to yIndex/tblUnk04 and re-measured
byte-identical at 98.05%.

WHAT IS LEFT, 46 of 2356 bytes:
  (a) FIVE .rodata POOL WORDS. The candidate emits its own address-constant
      pool, relocating against .rodata with addends 0, 4, 8, 0xc and 0x10,
      where the ROM names gUnknown_08090B98, _B9C, _BA0, _BA4 and _BA8. Those
      five ROM symbols are five consecutive 4-byte incbins in
      data/rodata-0808F098.s -- that is, the ORIGINAL unit's own
      -fforce-addr pool, which the splitter gave invented names. This is the
      ordinary rodata-carve case: the promotion needs a "rodata" entry naming
      those five words.
  (b) ONE SWAPPED PAIR OF SPILL COPIES. The ROM copies [sp,#36]->[sp,#56] and
      then [sp,#32]->[sp,#52]; the candidate does the two the other way round.
      The following two pairs ([sp,#40]->[sp,#60], [sp,#44]->[sp,#64]) and the
      [sp,#72] store already agree. There is no struct copy in the source
      here: these are reload's own spill copies around the terrain `switch`,
      so the order is reload's, not the source's.

## wave 97

Base: wave-93 draft (98.05%, size-exact), unchanged (`sub_0802AA78.w97-start.c`). No improvement.

Mechanism found with `-da` (tools/rtldump.py). The four "spill copies" are NOT reload's: the loop pass (dump.loop; the
function has no loop, the insns are created there) emits, before the terrain `switch`, one fresh pseudo per narrowed
variable holding its `(s16)` shift: `1532 = x<<16`, `1533 = y<<16`, `1534 = cx<<16`, `1535 = cy<<16`, `1537 = sel<<16`
(source pseudos 135/138/141/... are the expand-time shifts of the `(s16) x, (s16) y, ...` arguments in the
sub_0802A8DC call). Reload gives slots by ascending pseudo number: 135 -> [sp,#32], 138 -> #36, 1532 -> #52, 1533 -> #56.
The ROM's first copy is (#36 -> #56), second (#32 -> #52): i.e. the FIRST copy insn carries the HIGHER source and the
HIGHER destination pseudo, for the x/y pair only (later pairs agree).
Measured (build/probe/w97f.py, w97g.py): all 342 placements of `x` and `y` in the local declaration list give one of
`l32 s52` (126), `l32 s56` (54) or `l32 l36 ...` (162) as the first copy; NONE starts with the `l36` load. Swapping the two
declarations moves only the DESTINATION slots (l32 s56 l36 s52). So the copy order is x-first whatever the variable
numbers, and the source-shift pseudo of x (135) is numbered below y's (138) because the call's arguments are expanded
left to right. To reproduce the ROM the y shift would have to be EXPANDED before the x shift (an earlier `(s16) y`
use) while keeping the copy insns in x-then-y... no such earlier use exists in the source and adding one changes code.
Not found: a spelling that swaps only that pair. Pre-registration: partly held (slots do follow pseudo number, but the
pseudos are loop-pass temps, so declaration order cannot reach them).
Proposed summary: status 98.05% size-exact; left: five .rodata pool-word names (splitter case) and the first two of the
four pre-switch shift copies emitted y-before-x; tried: all 342 declaration placements of x and y (copy order never flips, only destination slots swap).

## wave 97 (W97-L) -- copy-order residual SOLVED, only the compiler-made pool-word names remain
Lever 2 (a signed running variable keeps a truncation): declare `s16 y;` instead of `u16 y;` (x stays u16).
With that the loop pass no longer makes a fresh shift copy for y in the swapped order; the two-copy pair now
matches. 98.05% -> 98.22%; CORRECTION (orchestrator): the remaining diff is NOT only names -- see the register block below (`.rodata+0/4/8/12` words vs ROM gUnknown_08090B98..BA4, gPlayers vs gUnknown_08499598, local branch
labels). `s16 x; s16 y;` gives the same 98.22%. Final source is work/sub_0802AA78/sub_0802AA78.c (u16 x, s16 y).
Negatives (size -4, 15.6%): binding `(s16) y` to a local `int ty` / `s16 ty` before the calls.
Proposed summary: status 98.22% size-exact, code stream identical to the ROM; left: five .rodata pool-word
names (compiler-made force-addr words, splitter case).

### wave 97 correction and second round (W97-L)
Remaining REAL instruction differences (with s16 y): three blocks near 0x540-0x774 where the loaded unit-array
pointer is a separate register: ROM `ldr r2,=X; ldr r1,[r2]; adds r3,r1,r0` vs draft `ldr r4,=X; ldr r3,[r4]; adds r3,r3,r0`,
plus `adds r5,r4,#4` (tblUnk04) placed after the sum in the ROM. Tried: statement order (e1 before tblUnk04) fixes the
`adds r5,r4,#4` position and the address-register choice in block 2 but leaves the pointer tied to the sum register
(97.45%, worse overall: it shifts later blocks); per-block `ub = gUnknown_08499594` local: block 3 alone 97.92%, blocks 1+2
-12 bytes (the held pointer is not reloaded after the call); index spelled `p + idx`, byte-cast `* 12`, array-cast,
`+ 0`: all byte-identical to the draft. So the split is local-alloc register choice for the load temp, not a
source-visible pseudo.
Permuter (900 s, 2 threads, from the s16-y source): NO-IMPROVEMENT. Final source: work/sub_0802AA78/sub_0802AA78.c
(= sub_0802AA78.w97L-e.c, 98.22%). Not run through wrongc.py because it differs from the wave-93 draft only by the
`u16 y` -> `s16 y` declaration.

## wave 97 (W97-W)
Aliases: gUnknown_08499594 (the unit array) is not aliased; gPlayers/gMap/gGameClock alias uses are not in the differing blocks (the diff sits in the e1/e2 blocks, draft lines 154-165). ROM detail: the second block reloads the pool word into a DIFFERENT register than the first (`ldr r2,=X` then `ldr r4,=X`), pointer in r1, sum in r3; the draft has r1/r1 with the pointer and sum tied to r3. Spellings tried (w97wB/C/D.c): index via a local `t` in all blocks (98.17%), `(u8 *)ptr + idx * 12` (98.22%, identical), volatile-pointer read (98.22%, wrong-C style, discarded). None changes the register choice. Draft unchanged (98.22%).

## wave 97 (W97-PG)
Permuter chain: 3 links, 98.22% -> 98.51%. Kept: link1 reuses zero as (s16)x for the x index in the tail; link2 q = cx then q = (q + ...) in the hp branch. Link3 NO-IMPROVEMENT. Start files .w97pg-perm1/2/3-start.c. Note link1 dropped the yIndex/tblUnk04 comment.
