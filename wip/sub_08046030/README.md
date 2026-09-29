# sub_08046030

0x08046030, 1556 bytes, THUMB, parked.

Best score so far: 92.3%.

## What it does

Sets up a per-army statistics screen (the draft calls it the results screen). It draws one row per army with its unit count and units lost, then captures, income and funds or placeholder text depending on defeat, AI control and fog, and stores each army's share of the counted terrain as a percentage.

## How close it is

Compiles to the right size (1556 bytes) with 92.4% of bytes identical. Naming the text buffer gBG0TilemapBuffer (the linker alias of gUnknown_08499578) in the five icon-row calls makes the compiler load its address again there, as the original does. What is left: 0x8000 and 0 are still shared with the first two calls, and the alias adds a second pool word, which shifts later pool offsets by 4.

## What is left

After working out the text column `a`, the original re-creates the three values shared by the next five text calls (the text-buffer pointer, 0x8000 and 0) where the draft keeps the earlier copies alive, and it stores two loop temporaries in the opposite stack slots. Find a spelling that gives each group of calls its own copies of those values without pinning registers.

## Already tried

- Setting `a` with an if statement instead of `?:`: loads in the wrong order; the `?:` form fixed 4 bytes and is kept.
- Reading the compared global into a local first: right load order but an extra copy the original lacks, and still one copy of each shared value.
- Fixing block-local variables to the original's registers: right registers, but their setup moves outside the first call.
- Assembler-symbol aliases for the shared values: forces the missing reload but splits the literal-pool entries wrongly.
- Register pins on the first group's shared values (the saved best): 4 bytes too long, 42.7%, not usable as source.

## Files

- `sub_08046030.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 1548/1556 (-8), 41.6%, pool 57/57, first real difference +0x188. Fixed block-local allocnos reproduce desired r6/r5/r4 but schedule initialization outside the first call; assembler-symbol aliases force the missing reload while incorrectly splitting literal-pool identity. Baseline restored.

### Wave 95

Base: the old draft (kept as `sub_08046030.w95-start.c`, 41.65%, size -8, first difference +0x188). Draft now 42.16%, size -8, first difference +0x188 (unchanged).

Residual re-read from the diff: block A (the five `sub_08014A5C` icon rows after the `a` chain) in the ROM holds a PLAIN `ldr r6,=gUnknown_08499578` (the variable's own address, used as `ldr r2,[r6]` five times) plus 0x8000 in r5 and 0 in r4 -- fresh pseudos. Before that chain the ROM reaches the same variable through the `.rodata` force-addr word (`ldr r7,=word; ldr r2,[r7]`). The draft reuses the word-derived pseudo (r7) for block A. So the ROM's later region does not share the earlier word-derived value; the ROM of `sub_08046914` shows the same pattern (forced word in the first three calls, plain `=gUnknown_08499578` in the else arms).

Transfer test from sub_08046914: its levers (constant locals `hi`/`zero` for the first three calls only; `static inline` text-draw helper for the calls after the first `if`; `u16 **pp = &gUnknown_08499578` bound before the later calls) did NOT produce the ROM's plain-literal re-creation there either (see that function's notes), so no lever was transferred. On this function:
- `static inline void DrawIcon(x, y, id)` wrapping the five block-A calls: 41.65 -> 42.16%, size unchanged. Kept (byte-equivalent C, small gain, no new cost).
- `pp = &gUnknown_08499578` bound before the five calls, or right after the `a` chain, and `(*pp)` in them: byte-identical to the draft (cse folds it). Kept as `sub_08046030.w95-pp.c`.
Conclusion: the two functions share the SAME unexplained construct (a later region whose text-buffer address is a plain literal while an earlier region's is the `.rodata` word); no source respelling tried creates the split. It is not a live-range problem in the draft (r7 is free in the ROM there).

Not run: permuter (1,556 B; would compete with the sub_08046914 run for cores, and the first difference at +0x188 is a create-a-pseudo residual).

Proposed summary:
- does: sets up the results screen: counts terrain by owner, then draws one row of numbers and icons per army
- status: 42% at -8 bytes; everything up to +0x188 is identical
- left: after the `a` chain the ROM re-creates the text-buffer address (plain literal), 0x8000 and 0 for the five icon rows; the draft keeps the earlier copies. Two loop temporaries are in swapped stack slots
- tried: statement-level respellings of `a`; `static inline` helper around the five rows (+0.5%); binding `&gUnknown_08499578` to a local (byte-identical)

### Wave 97

wave 97
Base: sub_08046030.c (== w95 draft, 42.16%, size -8, first diff +0x188). `best.c` (42.56%, +4) not adopted (reads-before-set tag, size +4).
Found the mechanism by RTL dump (`tools/rtldump.py sub_08046030 --flags=-da`): in the DRAFT, block A's `.LC` address load, 0x8000 and 0 are fresh pseudos in the `.jump` dump but `.cse` rewrites them to the pseudos of the first two calls (regs 297/300/302 reused across the label after the `a` chain). That is cse's skip-blocks path: `if (x <= 0x63) a++;` is a branch-around whose label has ONE reference, so cse follows the fall-through path and carries its table over the skipped block, invalidating only what the block sets (`a`). The ROM has the same shape (each chain label is referenced once, checked in target.s), yet it did not carry the table. So the ROM's cse either stopped before the chain or the block was not skippable.
Tested chain spellings (none changed the size or moved the first difference, 41.9-42.2%): `a = 0x17; if (..<=9) a = 0x18;` plus `a++` form; if/else form; `if/else` with an empty `else` arm; `a += (x <= 0x63)`; `a = c ? a + 1 : a`. Volatile read of the pointer inside DrawIcon: byte-identical (the address pseudo, not the load, is what is shared).
Open and testable next: what makes the ROM's cse stop before block A. Candidates not tried: a call or label between the second call and the chain that the ROM's source had (the wave-56 chapter says CFG is the same), and a limit on cse path length. Everything up to +0x188 matches.
Proposed summary: does = draws the results screen; status = 42% at -8 bytes; left = ROM re-creates the text-buffer address, 0x8000 and 0 after the `a` chain (cse does not carry them across it); tried = chain spellings (six), DrawIcon helper, pointer bind, volatile read (all no change).

wave 97 (W97-R)
Base: sub_08046030.w97-base.c (== the wave 95 draft, 42.16%, size -8, first difference +0x188).
LEVER FOUND: gUnknown_08499578 and gBG0TilemapBuffer are the SAME address (aw2bhr.lds line 34 defines the second as an alias). agbcc sees two different symbol_refs, so cse cannot equate them. Naming the text buffer `gBG0TilemapBuffer` in the DrawIcon helper (the five block-A calls) only, and leaving `gUnknown_08499578` in the earlier calls and in the k-loop, gives 91.07%, SIZE EXACT (1556), first difference still +0x188. It removes the size gap (-8 -> 0) and the re-creation of the text-buffer address the ROM shows after the `a` chain.
Mechanism confirmed with flags: `-fno-cse-skip-blocks` alone gives the ROM's block A form (fresh address load, fresh 0x8000, fresh 0), `-fno-cse-follow-jumps` alone does not. So cse's skip-blocks path (branch around `a++`) owns the carry; the alias only breaks the ADDRESS half of it. The 0x8000 and 0 constants are still carried over (block A uses the first pair's registers), which is the remaining residual: the ROM's block A has its own 0x8000/0 pseudos (r5/r4), ours reuses r6/r5. The ROM also has ONE pool word for the address (shared by the first pair and block A); the alias makes a second pool word, so the pool is one word longer than the ROM's and every later `ldr [pc,#N]` is off by 4 (that is most of the 139 differing bytes).
Negatives (all leave the carry in place, byte-identical or worse): chain as goto / empty-then / && form / `a += (x<100)` / `a = c ? a+1 : a` / do-while(0) or for(;;)/break/switch around the chain or the icons / 10-statement body in the skipped block (skip_blocks does not care about block size) / other spellings of `<= 0x63` / `a` as s8/s16 (adds extension insns, -4) / local `u16 t = gUnknown_03004080` (44.3%, -4, still carried).
What is NOT known: which source construct makes the ROM's cse decline BOTH skips (the reused `ldrh` after the first skip could instead be gcse's).
Proposed summary:
- does: sets up the results screen: counts terrain by owner, then draws one row of numbers and icons per army
- status: 91% at the right size; first difference +0x188
- left: the ROM's five icon rows load 0x8000 and 0 into fresh registers and share one pool word for the text-buffer address; the draft reuses the first two calls' 0x8000/0 and needs a second pool word for the alias
- tried: alias name for the five icon rows (+49%), six spellings of the `a` chain, loop/switch wrappers, type of `a`, local copy of the digit count

wave 97 (W97-W)
Base: work draft (92.35%, size-exact, first diff +0x188). Read the ROM pool: it has ONE word for gUnknown_08499578 in the first pool chunk (0x080462F0) and loads it TWICE from that word (r4 for the two calls after the `a` computation... i.e. before, r6 for block A). So the ROM's source used ONE name and cse simply did not merge the two loads; the alias reproduces the reload but adds a second pool word (the remaining +4 shift of later literal offsets). There is no placement of the alias that avoids the extra word: any use of the second name gets its own pool word.
Constants: block A's 0x8000/0 were respelled as `u16` parameters, `(u16)0x8000`/`(u8)0`, `u16` locals, `0x4000*2`/`1-1` (in `w97w.c`, VARIANT 1-4): all byte-identical to baseline (front end folds them; cse still shares). Draft unchanged. The open question is what makes cse in the ROM not carry table entries across the two branch-arounds (address, 0x8000, 0 together); the alias only breaks the address.

</details>
