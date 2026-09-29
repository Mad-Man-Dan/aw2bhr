# sub_0802AA78

0x0802AA78, 2356 bytes, THUMB, parked.

Best score so far: 98.5%.

## What it does

Draws the map information panel for the cell under the cursor: the terrain box, and the unit standing there with its HP, fuel, ammo and cargo. It then shows one of three small icons with a number, and sets display window 0 around the panel.

## How close it is

Compiles to the right size (2356 bytes) with 98.5% of bytes identical. What is left: in three blocks the original loads the unit-array pointer into its own register before adding the index; the draft loads it straight into the sum's register.

## What is left

Two spots remain. In one if/else pair the original loads two tables in the opposite order in both arms, but writing both arms that way lets the compiler merge them and lose 4 bytes, so only one arm is written that way; and at the end the original sign-extends the icon value q and reuses it, which our build optimises away. Both need a new idea.

## Already tried

- Reordering the table sum in both arms of that if/else: 4 bytes short (2352). Only one arm is reordered in the draft.
- Holding the shared sum in a local in one or both arms: much worse (as low as 43% identical).
- Declaring q as int, as u16, or as s16 with casts, in different scopes: no change, or 4 bytes too long for u16.
- Two chained permuter runs, over 11,000 attempts: no match.
- Older higher scores (92.5%, 93.9%) came from a best.c with the headers pasted in, not from this draft; they cannot be reproduced and must not be used as a starting point.

## Files

- `sub_0802AA78.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

Wave 74 strongest semantic draft: exact 2356/2356, 89.3%, 253 differing bytes, first difference +0x90.

### What still differs

A coupled GCSE/pool-order and q-allocation residual remains around the sub_0802BB74/sub_0802BAFC branch and later merge. The historical 92.53%/176 and apparent 93.9%/144 records came from expanded-header contamination, not a reproducible ordinary draft.

### Why it is close

Control flow, canonical Map view, call sequence and total size are settled; Wave 74 improved the honest 87.9%/286 baseline to 89.3%/253.

### Already ruled out

- Both-arm reassociation shrinks to 2352; then-arm-only association is retained.
- Two clean chained --current permuter runs, over 11000 iterations total, found no match.
- Do not seed from the contaminated ~198KB best.c or reshape Map/Tbl49A2A6.

### Settled

- Retain the then-arm `(cx + tbl) + gUnknown_0849A284[6]`, unit->unk07 binding, and zero-lifetime accv compare temporary.

### Why it is parked

Wave 74 W74-B. Resume with a new GCSE/merge-allocation mechanism from the include-based active draft.

### Wave 93

WAVE 93 (W93-D): 89.26% -> 98.05%, still size-exact at 2356 bytes, 46 differing bytes, first difference +0x90. The park said two clean chained permuter runs over 11,000 iterations found nothing; that was true of the run, not of the method. FIVE runs, each started from the previous one's kept improvement, gave 89.26 -> 93.80 -> 95.84 -> 97.28 -> 98.05, and a sixth found nothing. Every gain is a BINDING LOCAL or a REASSOCIATION; no statement was added, removed or reordered. The kept changes, each read and checked: the first sub_0802BAFC argument in the hp branch bound to `q` (a dead-range reuse -- q is assigned 0 later, before its own first read); gUnknown_0849A284[1] indexed through a local holding 1 (renamed yIndex); the table's unk04 column base bound to a local (renamed tblUnk04); `accv != zero` written as `accv != 0`, the same test because zero = 0 is assigned earlier; and three address sums reassociated. Both permuter temporaries were renamed out of new_var form and re-measured byte-identical. drafts.py bases reports no read-before-set and names the draft as the base. WHAT IS LEFT, two things. (a) FIVE .rodata POOL WORDS: the candidate emits its own address-constant pool relocating against .rodata with addends 0, 4, 8, 0xc, 0x10, where the ROM names gUnknown_08090B98, _B9C, _BA0, _BA4 and _BA8 -- five consecutive 4-byte incbins in data/rodata-0808F098.s, i.e. the ORIGINAL unit's own -fforce-addr pool that the splitter gave invented names. This is the ordinary rodata-carve case; the promotion needs a "rodata" entry naming those five words. (b) ONE SWAPPED PAIR OF SPILL COPIES: the ROM copies [sp,#36]->[sp,#56] then [sp,#32]->[sp,#52]; the candidate does the two the other way round, while the following pairs [sp,#40]->[sp,#60], [sp,#44]->[sp,#64] and the [sp,#72] store already agree. There is no struct copy in the source there -- these are reload's own spill copies around the terrain switch, so the order is reload's, not the source's. NOTE for the next wave: the 92.5%/93.9% figures this entry warned about as header-expanded contamination are now beaten by an honest include-based draft.

### Wave 97

wave 97
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

wave 97 (W97-L) -- copy-order residual SOLVED, only the compiler-made pool-word names remain
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

wave 97 (W97-W)
Aliases: gUnknown_08499594 (the unit array) is not aliased; gPlayers/gMap/gGameClock alias uses are not in the differing blocks (the diff sits in the e1/e2 blocks, draft lines 154-165). ROM detail: the second block reloads the pool word into a DIFFERENT register than the first (`ldr r2,=X` then `ldr r4,=X`), pointer in r1, sum in r3; the draft has r1/r1 with the pointer and sum tied to r3. Spellings tried (w97wB/C/D.c): index via a local `t` in all blocks (98.17%), `(u8 *)ptr + idx * 12` (98.22%, identical), volatile-pointer read (98.22%, wrong-C style, discarded). None changes the register choice. Draft unchanged (98.22%).

wave 97 (W97-PG)
Permuter chain: 3 links, 98.22% -> 98.51%. Kept: link1 reuses zero as (s16)x for the x index in the tail; link2 q = cx then q = (q + ...) in the hp branch. Link3 NO-IMPROVEMENT. Start files .w97pg-perm1/2/3-start.c. Note link1 dropped the yIndex/tblUnk04 comment.

</details>
