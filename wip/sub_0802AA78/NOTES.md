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
