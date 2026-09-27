# sub_0808AAF4 — flash read-ID (152 bytes)

## Wave 93 (W93-A): 59.87% -> 92.76% -> 94.08%, size-exact at 152

Two steps. First, the better of the two files already sitting in the work
directory was installed (the wave-88 draft, 92.76%). Then one 900 s /
4-thread permuter run on the DEFAULT profile improved it to 94.08%.

## The configuration contradiction is settled — DEFAULT -O2

The entry contradicted itself. On the same source:

| profile | score | size |
|---|---|---|
| configured (default -O2) | **92.76%** | exact, 152 |
| o1-no-force | 28.29% | -4 |

The wave-88 note calls o1-no-force "the profile ALL sixteen matched flash
entries actually use". True of those entries, false of this function: the -O1
run in `data/compiler-overrides.json` begins at sub_0808AB8C, this function
sits before it, and sub_0808AD6C inside the same span carries only
`cflags_remove ["-fforce-addr"]` at -O2. The flash library is several files,
not one. **No override is needed here, and it should not be measured at -O1
again.**

## The permuter's mutation is sound — audited

It added a second pointer to the same counter, `ptest = &(*p);` (so
`ptest == p`), whose only use is the SECOND delay loop's exit test,
`if (*ptest != 0)`. `p` does not change inside that loop, so the test is
identical at runtime. Renamed from `new_var` and re-verified at 94.08% after
the rename.

## The obvious hand fix is dead

The wave-17 "one local where the original had N" lever does not reach this.
All byte-identical to the 92.76% baseline:

- splitting the single `vu16 *p` into `p1` and `p2`, one per delay loop;
- splitting `int v` into `v1` and `v2`;
- doing both.

CSE knows every one of them holds the same stack address and coalesces them
back, so the number of SOURCE variables is not the number of pseudos here. The
wave-89 splitter list agrees: the fold-proof mask splits cse's value numbering
of an address computation with a NARROW VARIABLE OPERAND, and `sp + 64` has no
variable operand at all. W79's other direction — two block-scoped `vu16 i`
objects — gives two stack slots and +4. So the two live ranges have to come
from one object AND one source variable.

## Residual: 9 of 152 bytes

The ROM opens `push {r4, r5, lr}` and recomputes `add r1, sp, #0x40` before
EACH delay loop, carrying two pool words for 20000. The candidate still
computes that address once and keeps it in a callee-saved register across both
`bl _call_via_r5` calls.

Next: chain another permuter run from the 94.08% draft. This was the first run
ever made on this function and it improved on its first attempt.
