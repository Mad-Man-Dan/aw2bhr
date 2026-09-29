# sub_080359A4

0x080359A4, 324 bytes, THUMB, parked.

Best score so far: 79.3% (best.c).

## What it does

Draws a map unit's sprite when the tile it stands on is on screen, visible and occupied. It rejects anything outside the visible window by comparing the unit's position against the camera origin in gMap, and in one mode scrolls the camera to follow it instead. sub_080255F4 decides whether the unit may be drawn and sub_0801C254 places the sprite.

## How close it is

Compiles to the right size (324 bytes) with 77.2% of bytes identical. Assigning `py2 = py` on both paths after the call is the valid form of what the automatic search found. What is left is which registers hold proc, py and y.

## What is left

Find a way of writing it that makes the compiler put the x-position pointer in one of the high registers, as the original does, instead of a low one.

## Already tried

- Reaching the map through the compiler's own address word by hand (`&gUnknown_08090EA8` and `**screen`): replaced by naming gMap directly, which is what the original wrote and is worth 4 points and 4 bytes.
- Pinning the first y value to a fixed register: this was in the draft since wave 71 and now costs 4 bytes. Removed; the function became the right size.
- Writing the body with direct member access instead of the three pointer locals: 24 bytes short. The pointer locals are needed.
- One 15-minute run of the automatic permuter: 1.5 points better and still the right size. It wraps the camera call in a do/while that runs once, which changes nothing about what runs but stops the compiler reusing one register.

## Files

- `sub_080359A4.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 320/324 (-4), 45.1%, improved from 308 bytes using a private-slot local and fixed-ip y. Semantic body and indirection are settled; remaining four bytes are pointer allocation/copy placement. Broader private-slot lifetime probes regress the register rotation. WAVE 81 (D): profile sweep complete -- o1/o1-no-force worse (21.6%, +8 bytes), old-agbcc(-no-force) 40.7%, default/no-force both 45.1% (320/324); no configuration lever on this function.

### Wave 92

- **agent:** W92-A
- **measured:** 324/324 SIZE-EXACT, 56.17%, first difference +0x2, up from 320/324 (-4), 45.06%.
- **moved:** - The pool word. gUnknown_08090EA8 holds 0x08499590, the address of the map pointer, so the ROM's `ldr rN,=word; ldr r0,[rN]; ldr r3,[r0]` is agbcc's ordinary -fforce-addr chain and not a third level of indirection. `gMap->` emits it by itself, exactly as the matched neighbour src/decomp/c_080358C4.c does with its own word at 0x08090EA4. 45.06% -> 49.38%.
- Removing the `register int y asm("r12")` pin. It was added in wave 71 and was worth 12 bytes against the pool-chasing draft; against the gMap draft it COSTS the last four bytes. Removing it makes the function size-exact (49.38% -> 54.63%) and also un-blocks the permuter, which cannot parse the pin at all and had been reporting 'could not score the starting point' -- a syntax error that reads like a result.
- One permuter run, chained from the size-exact draft: 54.63% -> 56.17%, still size-exact. Its only meaningful change is a zero-trip do/while around the optional camera call; left untidied.
- **general_lesson:** WHEN A POOL-WORD SPELLING IS FIXED, RE-TEST EVERY ALLOCATION HACK THE OLD DRAFT CARRIED. They were tuned against different code, and here one was worth -4 bytes and a blocked permuter.
- **left:** Allocation. The ROM keeps &proc->unk42 in the HIGH register r8 and pays a `mov rLow,r8` at each of its three reads while keeping the py2 copy low; our build does the opposite. The ROM also reaches proc->unk35 as `&proc->unk42 - 13` and makes the py-to-py2 copy on both sides of the optional camera call where ours makes it once.
- **refuted:** - Writing the body with direct member access instead of the py / px / py2 pointer locals: 300 bytes (-24), 5.25% with the pin and 308 (-16), 7.10% without. The pointer locals are load-bearing.
- Reading permuter.log's 'found a better score!' lines as candidates. The two best scores (3500, 3580) verified at 34.26% and 39.20% and were 8 bytes short; the winner sat at 5100 against a 5400 base.
- **permuter_run_2:** Chained from 56.17%: reached 79.32% size-exact and was REJECTED. It deletes the unconditional `py2 = py;` and assigns py2 only inside the do/while that runs when the camera call runs, so on the other path py2 is read uninitialized (in the occupancy test and in sub_080255F4's third argument). Same class wave 74 rejected twice. Kept at work/sub_080359A4/w92-perm2-uninit.c. Two safe spellings of the same idea were measured and are worse: the copy written in both places is 328 bytes (+4), 18.90%, and deleting py2 entirely is 316 (-8), 14.51%. WHAT IT SHOWS: the ROM's two `adds r5,r4,#0` are ONE source statement duplicated by the compiler across the join, not two statements -- so the next wave's question is what makes agbcc duplicate one unconditional copy into both predecessors, which is much narrower than 'the pointer allocnos are rotated'.

### Wave 96

Base: `sub_080359A4.w96-start.c` (56.2%, first diff +0x2). `best.c` (79.3%) is wrong C: it assigns `py2 = py` only inside
the `if` and reads it after. Its content is that the copy `py2` must be created on the call path AFTER the call. The legal
spelling assigns `py2 = py` on BOTH paths (`if (...) { call; py2 = py; } else { py2 = py; }`): 75.3%, first diff still +0x2
(the prologue register order). Transferred as a general trick: an "uninitialised on one path" permuter form has a legal
twin that assigns the copy on every path. Then one 800 s permuter run: 75.3% -> 77.2%, kept change is a write-only temp
around the first call argument (`sub_080358C4(pxVal = *px, y)`) and `(*py)` instead of `(*py2)` in the turnState read.
Residual: register roles. ROM: proc r6, py r4, y in ip, px address r8, `*px` value r7, py2 r5, and
`proc->unk35` computed as `px - 13` off the r8 copy of `&proc->unk42`; ours: proc r5, py r6, y r4, px address sl.
Reading `unk35` through `((u8 *)px)[-13]` is byte-identical (cse already derives it). Not solved.
Proposed summary: 77.2% size-exact; left: register roles for proc/py/y/px, first diff in the prologue.

### Wave 97

wave 97 (W97-U)
Base `sub_080359A4.c` (77.16%). New fact from reading the ROM: after the do-while every later read of y goes through the
COPY (r5 = py2), including `((*py2)+8)/16` and the final call's y argument; py (r4) is used only to make the copies, and
`y` (first read, kept in ip) is a separate pseudo held in a high register. Two live pointer copies + a high-register y is
why the ROM pushes only {r6,r7} for hi regs: spelling BOTH later reads as `*py2` makes the push list match
(`{r6,r7}`) but copy propagation deletes py2 entirely (316 B, -8, 15.1%). Reading only the final call's y through py2:
316 B, 13.0%. `py2 = py;` before the `if` and again after the call: the second copy is deleted as redundant (320 B, -4);
with both later reads `*py2` that is 59.6%, push list matches, size -4 (the ROM's second `adds r5,r4,#0` after the call
is what is missing). `x = *px` as a real s16/int local used in the range checks: 53.4% +4, frame differs.
Mechanism: the ROM keeps two copies (before and after the call); cse removes the second because the first already
makes py2 == py. No spelling tried keeps both; whatever the source does, the copy after the call must not be provably
redundant (the C for that is unfound).

</details>
