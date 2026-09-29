# sub_08046030

0x08046030, 1556 bytes, THUMB, parked.

Best score so far: 42.6%, +4 bytes (best.c).

## What it does

Sets up a per-army statistics screen (the draft calls it the results screen). It draws one row per army with its unit count and units lost, then captures, income and funds or placeholder text depending on defeat, AI control and fog, and stores each army's share of the counted terrain as a percentage.

## How close it is

Compiles 8 bytes short (1548 against 1556), 42.2% of bytes identical; the first 0x188 bytes match. A static inline helper around the five rows of the first text block helped slightly. What is left: the original reloads the text-buffer pointer, 0x8000 and 0 for each group of text calls where the draft keeps one copy.

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
- `best.c`: the closest attempt, when it is not the draft
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

</details>
