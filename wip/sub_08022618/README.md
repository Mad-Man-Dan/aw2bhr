# sub_08022618

0x08022618, 400 bytes, THUMB, parked.

Best score so far: 61.2%.

## What it does

Draws the 2 x 2 status icon for the unit on map tile (x, y) into a background tilemap buffer, at the tile's position relative to the camera. The top-right tile carries a low-ammo / low-fuel mark; if there is no suitable unit it hands the tile to sub_080225CC instead.

## How close it is

Compiles to the right size (400 bytes) and about 60% of the bytes match. The instructions are the right ones; what is left is that the compiler puts several values in different registers from the original, which shifts every byte after the first disagreement.

## What is left

Find a way of writing the top of the function (the x/y parameters and the two screen offsets) that makes the compiler give x and y the low registers.

## Already tried

- Map access through struct Map members instead of byte offsets: fixed the size (was 4 bytes short).
- Assigning id inside the first test, flags as its own variable, the offsets masked then doubled in two statements, and the two tables named directly: each moved it closer; all are in the draft.
- The offsets as one expression each, u16 id, other statement orders: worse.
- Two 15-minute permuter runs: best useful result was 51% but it breaks the id code the original has; the other best result changed the program's meaning.

## Files

- `sub_08022618.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

396 of 400 bytes, 4 SHORT, 11.8% identical, first difference at +0xa (wave 15, C; improved wave 79, W79-F). Straight-line, 2 `.LC` pool words (gUnknown_080909A8 -> &gUnknown_08499580, gUnknown_080909AC -> &gUnknown_085D5ABC), both spelled with A2's `pp = &<word>`; `pp` stays in sl as the ROM has it. Every statement, the || chain, the four 2x2 tile stores and the two Div clamps are identified. WAVE 79 (W79-F) RE-MEASURED AND IMPROVED IT FROM -8/10.0% TO -4/11.8%. Note the 31.0% this entry used to quote is best.c's, not the draft's. THE FIX: `id`'s zero-extension survives in the ROM because the pseudo is SET IN MORE THAN ONE BASIC BLOCK, not because of any cast spelling. Reusing one s16 for `id` and then for `flags` (legal -- id is dead after `rec = &gUnknown_08499594[id]`) recovers 4 of the 8 bytes. Eight spellings measured, all -8 except that one: u16 id single-set is -12 (everything folds, including an explicit (s16) at the use), s16 id single-set is -8, `int id` with (u16)/(s16) casts is -8, `int id = (u16)q[off]` is -8, `s16 id = (u16)q[off]` is -8, and MANUFACTURED second stores (`id = id;`, `id &= 0xff;`, `id = 0;` ahead of the real store) are all bit-identical to single-set because flow deletes them before combine. THE REMAINING 4 BYTES are the price of that fix: the merged pseudo now spans two blocks and spills `by` to a 4-byte stack slot (`sub sp,#4; str r0,[sp,#0]`) that the ROM does not have. The ROM holds x=r5, y=r4, ax=sl, by=sb, id/flags=r7, id<<16=r6, rec=r8. So the right answer is a second definition of `id` that does NOT lengthen its live range -- some variable already assigned in another block that I did not identify. POOL WORDS ARE NOT A RESIDUAL HERE, and this entry's old `four uses is enough` claim is in the wrong units: counted in target.s and the candidate .s, all four globals are named EXACTLY ONCE in BOTH, while the draft has three source-level *pp dereferences. Ruled out: old_agbcc (identical size); ax/by as dead entry parameters (identical output); 300 s of decomp-permuter; declaration order of the merged variable (both slots, identical 396/11.8%). Draft in work/sub_08022618/sub_08022618.c.

### Wave 91

WAVE 91 (W91-A). Member form POSITIVE: the p/rows/q byte-pointer locals respelled as ((struct Map *)gUnknown_08499590)->height/camX/camY/rowOffset[y]/unitUnk[off]/unk234A[off] take it from -4 to SIZE-EXACT (13.8%); a bound `struct Map *m` is byte-identical to the inline cast. Then, each measured: (1) ax/by in two steps (`ax = (x - camX) & 0xF; by = ...; ax *= 2; by *= 2;`) removes the sub sp,#4 spill -- one-expression ax lets CSE's associative shift fold turn the first tile's `ax << 16` into `t2 << 17`, keeping the masked intermediates live into the else block; (2) `if ((id = m->unitUnk[off]) == 0 || ...` reproduces the ROM's id shape exactly (`ldrb; lsls r0,#16; lsrs rV,r0,#16; cmp r0,#0`, then `lsls r6,rV,#16` at the call) -- u16 id is -24; (3) flags as its own s16; (4) the force-addr words named as what they hold, gUnknown_08499580 for **pp and gUnknown_085D5ABC[...] for (*pp2)[...] (rodata 0x080909A8/0x080909AC). Draft: 400/400, 29.0%, first diff +0xc, PURE ALLOCATION: ROM x=r5 y=r4 ax=sl by=sb, draft x=r8 y=r7 ax=r5 by=r4. Permuter 900 s from it: a size-exact 51.25% variant with static-inline helpers and `flags = (id = ...)` (work/sub_08022618/perm-w91-q1.c) that loses (2)'s id shape; a chained 900 s run's best raw (63.25%) is semantically wrong (`by = (ax *= 2)`). Evidence: work/sub_08022618/NOTES.md.

### Wave 93

WAVE 93 (W93-C). 29.00% size-exact -> 61.25% size+0, first difference +0xc. TWO steps, both audited.
(1) BASE. The orchestrator's base scan recommended `recovered.c` at 63.25% size-exact. It is the candidate W91-A already identified and discarded, and NOTES.md says so in as many words: it contains `by = (ax *= 2);`, which overwrites by with ax, so it computes the wrong tile row. Renamed `recovered.c.wrongc`. The scan's SECOND suggestion, `perm-w91-q1.c` (51.25%, size-exact), is sound C -- `flags = (id = ...)` sets flags before the test reads it, and the two `static inline` helpers are a row read and a (u16) narrowing -- so it was adopted and its `inline_fn` / `inline_fn2` renamed to `row_start` / `to_u16` (byte-neutral, re-verified at 51.25%).
(2) PERMUTER, two chained 900 s runs, each output audited before keeping. 51.25 -> 55.00: deleted the first `tile = ...` binding and spelled that one address out at the store. All four tile stores still address the same words and `tile` is still assigned before the two reads that use it, so there is no read-before-set; this is the wave-77 `sub_08073304` lever (delete a redundant copy statement) found by search rather than by reading. 55.00 -> 60.25: added `id = x;` at the top and used `id` for the column in `off = rowOffset[y] + id` and `sub_080225CC(id, y)` before `id` is reassigned to the unit id, so one local now carries two roles and two live ranges merge -- the same lever the original entry found between `id` and `flags`. `id` is written before every read, and a map column cannot reach 0x8000, so the u16-to-s16 narrowing is value-preserving. Both kept forms were reformatted into house style and re-verified byte-identical.
RESIDUAL is unchanged in kind and is pure allocation: the ROM keeps x and y in r5/r4 and ax/by in high registers, the candidate has the two swapped (x/y in r9/r8). The `.rodata` pool words are NOT a residual -- trymatch resolves the candidate's own two-word pool against the ROM's 0x080909A8 / 0x080909AC.

### Wave 97

wave 97 (W97-L)
Base: existing draft (61.25%, size-exact), restored as the final file. The draft aliases `x` into the unit-id
variable (`id = x`); the ROM has x in r5 and y in r4 as their own u16 pseudos (x is re-passed to sub_080225CC and
added into the row index), the id is a separate u16 loaded via `lsls #16; lsrs #16` with the zero test on the shifted
value, and (s16)id is a separate `lsls r6,r7,#16` copy used for both sub_0802571C's argument and the unit index.
Tried: x/y separate with `u16 id` (10.75%, -4), with the draft's `flags = (id = ...)` s16 spelling (56.00%, size-exact,
first diff +0xC), `int id` (53%, adds a frame). The ax/by doubled values sit in sl/sb from the start in the ROM
(x, y, id and the id copy fill r4-r7 first); in every separate-x spelling ax/by take r4/r5 and x goes to r8.
Permuter chained once from the 56.00% spelling: 59.75% (kept as sub_08022618.w97L-perm1.c, not adopted: below the draft).
Proposed summary tried: + "x and y as own variables (the ROM's shape) and a u16/int id".

</details>
