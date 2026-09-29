
## wave 95

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
