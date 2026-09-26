# sub_08022618

0x08022618, 400 bytes, THUMB, parked.

Best score so far: 48.8%, +20 bytes (best.c).

## What it does

Draws the 2 x 2 status icon for the unit on map tile (x, y) into a background tilemap buffer, at the tile's position relative to the camera. The top-right tile carries a low-ammo / low-fuel mark; if there is no suitable unit it hands the tile to sub_080225CC instead.

## How close it is

Compiles to the right size (400 bytes) but only 29% of bytes match. The logic and the order of the code are right; the difference is register allocation: x and y should sit in r5/r4 and the two screen offsets in high registers, and the compiler swaps them.

## What is left

Find a way of writing the top of the function (the x/y parameters and the two screen offsets) that makes the compiler give x and y the low registers.

## Already tried

- Map access through struct Map members instead of byte offsets: fixed the size (was 4 bytes short).
- Assigning id inside the first test, flags as its own variable, the offsets masked then doubled in two statements, and the two tables named directly: each moved it closer; all are in the draft.
- The offsets as one expression each, u16 id, other statement orders: worse.
- Two 15-minute permuter runs: best useful result was 51% but it breaks the id code the original has; the other best result changed the program's meaning.

## Files

- `sub_08022618.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

396 of 400 bytes, 4 SHORT, 11.8% identical, first difference at +0xa (wave 15, C; improved wave 79, W79-F). Straight-line, 2 `.LC` pool words (gUnknown_080909A8 -> &gUnknown_08499580, gUnknown_080909AC -> &gUnknown_085D5ABC), both spelled with A2's `pp = &<word>`; `pp` stays in sl as the ROM has it. Every statement, the || chain, the four 2x2 tile stores and the two Div clamps are identified. WAVE 79 (W79-F) RE-MEASURED AND IMPROVED IT FROM -8/10.0% TO -4/11.8%. Note the 31.0% this entry used to quote is best.c's, not the draft's. THE FIX: `id`'s zero-extension survives in the ROM because the pseudo is SET IN MORE THAN ONE BASIC BLOCK, not because of any cast spelling. Reusing one s16 for `id` and then for `flags` (legal -- id is dead after `rec = &gUnknown_08499594[id]`) recovers 4 of the 8 bytes. Eight spellings measured, all -8 except that one: u16 id single-set is -12 (everything folds, including an explicit (s16) at the use), s16 id single-set is -8, `int id` with (u16)/(s16) casts is -8, `int id = (u16)q[off]` is -8, `s16 id = (u16)q[off]` is -8, and MANUFACTURED second stores (`id = id;`, `id &= 0xff;`, `id = 0;` ahead of the real store) are all bit-identical to single-set because flow deletes them before combine. THE REMAINING 4 BYTES are the price of that fix: the merged pseudo now spans two blocks and spills `by` to a 4-byte stack slot (`sub sp,#4; str r0,[sp,#0]`) that the ROM does not have. The ROM holds x=r5, y=r4, ax=sl, by=sb, id/flags=r7, id<<16=r6, rec=r8. So the right answer is a second definition of `id` that does NOT lengthen its live range -- some variable already assigned in another block that I did not identify. POOL WORDS ARE NOT A RESIDUAL HERE, and this entry's old `four uses is enough` claim is in the wrong units: counted in target.s and the candidate .s, all four globals are named EXACTLY ONCE in BOTH, while the draft has three source-level *pp dereferences. Ruled out: old_agbcc (identical size); ax/by as dead entry parameters (identical output); 300 s of decomp-permuter; declaration order of the merged variable (both slots, identical 396/11.8%). Draft in work/sub_08022618/sub_08022618.c.

### Wave 91

WAVE 91 (W91-A). Member form POSITIVE: the p/rows/q byte-pointer locals respelled as ((struct Map *)gUnknown_08499590)->height/camX/camY/rowOffset[y]/unitUnk[off]/unk234A[off] take it from -4 to SIZE-EXACT (13.8%); a bound `struct Map *m` is byte-identical to the inline cast. Then, each measured: (1) ax/by in two steps (`ax = (x - camX) & 0xF; by = ...; ax *= 2; by *= 2;`) removes the sub sp,#4 spill -- one-expression ax lets CSE's associative shift fold turn the first tile's `ax << 16` into `t2 << 17`, keeping the masked intermediates live into the else block; (2) `if ((id = m->unitUnk[off]) == 0 || ...` reproduces the ROM's id shape exactly (`ldrb; lsls r0,#16; lsrs rV,r0,#16; cmp r0,#0`, then `lsls r6,rV,#16` at the call) -- u16 id is -24; (3) flags as its own s16; (4) the force-addr words named as what they hold, gUnknown_08499580 for **pp and gUnknown_085D5ABC[...] for (*pp2)[...] (rodata 0x080909A8/0x080909AC). Draft: 400/400, 29.0%, first diff +0xc, PURE ALLOCATION: ROM x=r5 y=r4 ax=sl by=sb, draft x=r8 y=r7 ax=r5 by=r4. Permuter 900 s from it: a size-exact 51.25% variant with static-inline helpers and `flags = (id = ...)` (work/sub_08022618/perm-w91-q1.c) that loses (2)'s id shape; a chained 900 s run's best raw (63.25%) is semantically wrong (`by = (ax *= 2)`). Evidence: work/sub_08022618/NOTES.md.

</details>
