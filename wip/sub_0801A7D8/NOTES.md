
## Wave 90 port (orchestrator)

Ported to PR #3 names with tools/port_rename.py; `gUnknown_0200CC88` became `struct SaveSlotGenerations`, so the five `(&gUnknown_0200CC88[16])[x]` uses were hand-ported to `(&gUnknown_0200CC88.slotGeneration[0])[x]`, which is byte-identical to the pre-merge candidate (verified against _cand.prepr3.bin). The plain member `gUnknown_0200CC88.slotGeneration[x]` CHANGES the bytes: 1068/1056 (+12, unchanged size delta), 23.2% against the port's 20.0% -- a small positional change, not adopted as part of the port. Worth re-trying as a lever once the +12 is solved (see W90-B's member-vs-pointer chapter in docs/agbcc-codegen.md).

## Wave 94 (W94-A) - adopted Vesly's draft, 19.76% at +12 -> 83.71% size-exact

**Our draft was the wrong C, not Vesly's.** Diffing the two statement by
statement left exactly one semantic difference: ours wrote
`unk20[cur] = (unk20[cur] | 8 | tag) & 0xfb` with `tag = idx << 4`, Vesly's ORs
a plain `int` zero. `target.s` settles it: line 413 is `movs r7, #0`,
immediately before the checksum stores, so r7 holds ZERO at the `orrs r0, r7`
on line 432. The wave-65 note in the park ("r7 is NOT zero ... the honest
expression is therefore `| (idx << 4)`") is REFUTED - the `lsls r7, r6, #4` on
line 233 is an earlier, unrelated use of r7, and r7 is redefined to 0 before
the OR. Everything else in the two files computes the same thing.

What bought the 12 bytes, both structural:

- the two 16-element int arrays are ONE local struct
  (`struct SaveSegments { int length[16]; int offset[16]; }`), not two
  separate locals;
- the 4 KiB staging buffer is reached through a cast struct view
  (`struct SaveSector`), not raw `*(u32 *)(buf + k)` stores.

**A promotion blocker to fix, not to copy.** Vesly's file renames two
`include/unknown-globals.h` declarations out of the way with `#define` /
`#undef` around `#include "global.h"` and redeclares the same symbols typed
(`u8 *gUnknown_0200CC2C`, `int (*gUnknown_0200CC24)(u8 *)`). That is two
`extern`s in a `.c`, which the standing rules forbid. It is load-bearing for
this draft as it stands; promotion needs those declarations moved into the
header.

Permuter, two chained runs, 83.24 -> 83.43 -> 83.71, size-exact throughout,
first difference stuck at +0x82. Three mutations, each audited equivalent:

- `length` reused as a scratch for `gUnknown_0200CD08` before the
  `<= (u32)-2` test (dead there; reset at the top of the outer loop);
- `unk00[i]` bound to a local (renamed `owner`) for its two equality tests;
- `writtenSlots[i]` bound to the dead `total`.

The scratch reuse of `length` and `total` is pseudo sharing and is
load-bearing. Do not tidy either into a fresh local.

### Residual

1056/1056, 172 bytes differ, 83.71%, first difference +0x82 - pure register
allocation in the sort loop. The ROM binds `freeSlots`'s stack base into `ip`
and the generation table's pool address into `r8`; the candidate has the two
swapped, and the low-register numbering follows from it. Both create the two
pseudos in the same order, so this is the allocator's tie-break rather than a
source-order lever.

## wave 97

Base: `sub_0801A7D8.c` (83.71%, size-exact, first difference +0x82), saved as `sub_0801A7D8.w97-start.c`. Final: **84.38%**, size-exact, first difference +0x82 (the residual is the sort-loop register swap and everything downstream of it).

Pre-registration ("struct MEMBER hoists its base register; an array subscript does not"): NOT the lever for the first difference. The save sector is already a struct view throughout, and the first difference is in the sort loop over `freeSlots`, which is a plain array.

Mechanism of the first difference, from `-da` (dump.greg): the sort loop holds two invariants, the `freeSlots` stack base (pseudo 95: 7 refs over 20 insns) and the generation table address (pseudo 100: 8 refs over 34 insns). The allocator priority is floor_log2(refs) * refs / live_length: 2*7/20 = 0.70 against 3*8/34 = 0.70, and the table is sorted first, so it takes the first hi register (ip) and the base gets r8. The ROM has them the other way round (base ip, table r8), so its base outranks its table. Only a reference-count change moves that: one more use of the base pseudo (8 refs gives 3*8/20 = 1.2) or one fewer of the table (7 refs gives 0.41).

Probed and negative, all byte-identical or worse (one-unit harness `build/probe/w97k.py`):
- the table bound to a local before the loop or inside the outer body (first difference moves to +0xc): binding makes the table a user variable hoisted and re-derived everywhere;
- an `fs = freeSlots` pointer (size +8); `&freeSlots[i]` held in a local (first difference +0x80, worse); `&freeSlots[j]` per iteration;
- comparison operands swapped (`>`): puts base in ip and table in r8 like the ROM, but turns `bcs` into `bls` and makes `&freeSlots[j]` the hoisted address; the ROM hoists `&freeSlots[i]`;
- generation values read into locals in either order; swap spelled from `[j]` first; a second temp for freeSlots[j]; `(int) freeCount` in the inner bound.

Permuter (3 chained 900 s x 2 runs, 83.71 -> 84.00 -> 84.19 -> 84.38): all value-preserving, none touch the first difference: a `do { } while (0)` around the swap store, `+ -1` for `- 1` in the segment tag, a `new_var` pointer binding of the sector view at the `checksum` store, `do{}while(0)` around one slotOwners store, and `lastByte = 0xfff` moved down to just before the `sub_0801B648` call (equivalent: it is only read in the failure arm). A `(segment ^ 0) >= 0` in the loop test was removed as not load-bearing.

Proposed summary: does = writes a save to flash sector by sector with a header, checksum and generation counter, retrying on spare slots; status = "1056 bytes, size exact, 84.4%; register allocation in the sort loop and after"; left = "the original holds the freeSlots stack base in ip and the generation table address in r8; ours holds them the other way round, and the rest follows"; tried = the list above and the priority arithmetic.
