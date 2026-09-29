# sub_0802F588

0x0802F588, 280 bytes, THUMB, parked.

Best score so far: 86.6%, +4 bytes (best.c).

## What it does

Sends one packet on the link. Into the 512-halfword send ring it writes a 0x4FFF start marker, the payload length in halfwords, a checksum (0x4FFF plus the length plus each payload halfword times its 1-based position) and the sum of the complements of those products, then the payload. It returns -1 as soon as writing would make the write cursor catch up with the read cursor, and otherwise stores the advanced cursor and returns the halfword count.

## How close it is

Compiles to the right size (280 bytes); 50 bytes differ. Everything up to the first checksum store is byte-exact; the difference is register choice from there on.

## What is left

Find what makes the compiler pick the original's registers in the payload-copy loop and the stores around it.

## Already tried

- Reaching the ring and the two cursors through the compiler's own address words by hand: replaced by naming gUnknown_0300410C, gUnknown_02025818 and gUnknown_030040CC directly, which is what the original wrote.
- Declaring the byte-count parameter as u16 rather than int: this is what puts the parameter's entry conversion in the prologue where the original has it, and it is worth 3 points. Every caller passes a small constant, so no call site changed.
- Folding the checksum's starting value into one statement: 4 bytes too long.
- One 15-minute run of the automatic permuter: 45% to 82% identical, still the right size. It copies the marker constant and the returned count into locals first; both copies are needed and neither changes what runs.

## Files

- `sub_0802F588.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

Wave 74 strongest semantic draft: exact 280/280, 40.4%, 167 differing bytes, first difference +0xe.

### What still differs

The ring-writer shape is correct but allocation remains broadly rotated across its three loops.

### Why it is close

Size and loop count are exact; the retained split-sum and reversed second-pointer equality are semantic and independently verified.

### Already ruled out

- Two clean permuter runs were drained.
- The apparent exact 45.7%/152-difference candidate used prod before initialization and was rejected.

### Settled

- Do not prefer best.json's stale 55.4% record over the verified semantic active draft.

### Why it is parked

Wave 74 W74-C. Requires a new allocation mechanism, not another unconstrained permuter adoption.

### Wave 92

- **agent:** W92-A
- **measured:** 280/280 size-exact, 82.14%, first difference +0x10, up from 40.36%.
- **moved:** - The three globals named directly. 0x08090C80/84/88 hold 0x0300410C, 0x02025818 and 0x030040CC and are ONE -fforce-addr address block with addends 0/4/8, not a ROM table of pointers; the draft was reproducing by hand what agbcc emits by itself. Naming gUnknown_0300410C / gUnknown_02025818 / gUnknown_030040CC gives the three-level chain for the early references AND the direct pool words for the later ones, which is the split the ROM shows. 40.36% -> 40.71%. This also disposes of the wave-49 note about a dead `ldrh` no pointer spelling produces: the dead load belongs to gUnknown_02025818's `volatile u16 []` declaration, and reached by its own name every store in the function gets it.
- Reading the write cursor BEFORE computing the halfword count. 40.71% -> 41.79%.
- THE SECOND PARAMETER IS A u16, NOT AN int. The ROM's `lsls r1,r1,#16` sits in the prologue five instructions before the `lsrs r6,r1,#17` that uses it; a cast at the use emits both shifts together, and only a sub-word PARAMETER puts the first in the prologue's own insn group, where combine merges the entry's `lsrs #16` with the source's `>> 1`. include/unknown-functions.h retyped. 41.79% -> 45.00%.
- One permuter run, chained: 45.00% -> 82.14%, still size-exact.
- **prototype_change:** include/unknown-functions.h: sub_0802F588's second parameter int -> u16. BYTE-NEUTRAL AT EVERY CALLER AND CHECKED, NOT ASSUMED: all ten call sites pass a small literal (4, 0x1a, 0x84), and sub_0802FA64, sub_080309AC, sub_08030D84, sub_080319AC, sub_08031A24 and sub_080320AC were re-run through trymatch after the retype and all six still MATCH.
- **permuter_changes_audited:** Two, both behaviour-preserving and both checked by hand. `new_var = 0x4fff; gUnknown_02025818[cur] = new_var;` is the fix for the residual the hand analysis had already named (the dead ldrh in front of that store was taking the register holding 0x4FFF, so our build reloaded the constant where the original keeps it live). `chk = n; return chk;` copies the returned count into the checksum accumulator, which is dead by then; n is at most 0x7FFF and chk is u16, so the copy is lossless. Both left untidied.
- **left:** Allocation, first difference +0x10.
- **refuted:** - Folding the checksum's starting value into one statement (`sum = n + 0x4fff;`): 284 bytes (+4), 30.99%. The two-statement form stays.
- **permuter_run_2:** Chained from 82.14%: reached 86.62% but FOUR BYTES TOO LONG, and tools/permute.py installed it over the size-exact draft because its keep rule compares percentage only and ignores the size delta. Restored to 82.14% size-exact; the rejected form is at work/sub_0802F588/w92-perm2-plus4.c. This is the SECOND time this function has been caught by the same trap -- the wave-74 entry records the first. Its one useful hint: it binds 0x1FF to a local in the payload loop, which is aimed at the ROM's single materialisation of that mask into a held register, but pays an extra instruction for it. FOR THE TOOLING: refuse to replace a size-exact draft with one that is not.

### Wave 96

Base: existing draft (82.14%, size-exact), kept as `sub_0802F588.w96-start.c`; draft unchanged.
The diff is two hi-register assignments swapped: the ROM keeps `chk` in r9 (zeroed in the prologue) and the ring base in ip
(`mov ip,r0` after `ldr r0,[cell]`); the draft has them the other way round, and the cascade moves cur*2 / cur+1 and the mask.
The final `chk = n; ... return chk;` copy is not in the ROM (it returns `adds r0,r6,#0`, i.e. n itself), but removing it (`return n;`)
drops to 60.7%: the copy is what keeps `chk` off the register the ROM leaves alone (see the header comment), so the +2 copies of the
pre-registration are NOT this one.
Negatives: declaring `chk` before `sum` and moving `cur` in the declaration list are byte-identical. Permuter, one 900 s run:
82.14 -> 82.50% but the kept change is WRONG C (`cur = chk; ... return cur;` stores n into the write cursor); rejected and the draft restored.
Proposed summary: does = writes one packet into the send ring; status = 82.1%, size-exact; left = chk (r9) vs ring base (ip) swapped;
tried = decl order, return n, one permuter run (its only gain was wrong C).

### Wave 97

wave 97 (W97-S)
Draft unchanged (82.14%). best.c (86.62%) is +4 bytes, not progress. Tried by spellings.py: `chk = 0` moved to just before the first loop 22.9% +8; `chk = chk - prod - 1` 43.7% +4; s16 chk 41% +8; s16 sum 17.7% +8; swapping the sum/chk statements in the loop 77.5% size-exact; `chk = 0` placed after `cur = ...` 77.5%. None moves chk to r9.

</details>
