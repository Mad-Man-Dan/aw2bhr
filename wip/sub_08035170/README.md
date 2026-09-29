# sub_08035170

0x08035170, 128 bytes, THUMB, parked.

Best score so far: 85.2% (best.c).

## What it does

Decides the weather for the next turn and returns it. It starts from gPlaySt.weather: if that is 0 (any other value is treated as 0) and the random-weather mode is 1, it rolls a percentage chance for weather 1 and then for weather 2 (sub_080129F8, with chances from gUnknown_03004490); if it is 1 or 2, it may switch back to the map's defaultWeather when sub_08035080 says so, depending on the random-weather mode.

## How close it is

Reports 128 of 128 bytes, but only because of alignment padding at the end: the code itself is 4 bytes short (one register copy near the start and one instruction at the end). 50.8% identical. The 85.2% that trymatch prints belongs to best.c, a different draft that reads an uninitialised pointer and must never be promoted.

## What is left

Across the call to sub_08035080 the original keeps the address of a compiler-made address word in a saved register and reloads through it afterwards; ours keeps the finished field address instead. That word has no name in C, so this is believed unreachable from source. The current draft carries no inline helper, so the permuter can run on it; no run on this exact draft is recorded.

## Already tried

- About twenty ways of writing the reads (plain or volatile at each site, reshaping the case 1/2 test, reading into locals or pointer locals, a pointer to the struct everywhere): the field address is still kept across the call.
- A static inline helper to re-read the field: identical output.
- Making the field offset volatile so the address cannot be reused: the compiler then keeps the struct's base address instead, still not the original's choice. The reuse was never the real problem.
- Width casts to break the reuse: folded away, no effect.
- Permuter, 19,333 attempts from best.c: no match (best 83.6%), and best.c is itself invalid.
- Declaring the address word as a real pointer global: adds a third load.
- Matching the callee sub_08035080 first: this function did not change at all.

## Files

- `sub_08035170.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

SIZE-EXACT (128/128). The DRAFT scores 50.8%, first difference at +0xc -- measured wave 59 (W59-B) by exit status. The 85.2% that trymatch prints as 'best so far' is best.c's score and is NOT this draft; best.c holds a permuter shape that reads an UNINITIALISED pointer on the case 1/2 path and must not be promoted.

### What still differs

ONE ADDRESS CSE ACROSS THE CALL. The ROM keeps &.LC -- the .rodata force-addr word's ADDRESS -- in r5 across the sub_08035080 call and re-derives the whole access afterwards (ldr r0,[r5] ; adds r0,#0x2f ; ldrb r4,[r0]); the draft keeps the finished address base + 0x2f in r5 and reloads only the value (ldrb r0,[r5] ; adds r4,r0,#0). The knock-on is the other 2 bytes: because the ROM's tail re-reads the force-addr word, &.LC is live across the call and gets a callee-saved register of its own (adds r5,r1,#0 right after the entry load, which is the first differing instruction at +0xc).

### Why it is close

The dispatch tree, both sub_080129F8 calls, the pool split, both case 1/2 guards and the size are the ROM's. The signed blt in the dispatch tree fixes the switch operand as int rather than u8; default: sits ahead of case 0: and falls into it. Settled since wave 45.

### Already ruled out

- Wave 51: decomp-permuter, 300 s, 19,333 iterations from the 85.2% best.c -- no byte match, ceiling 83.6%. Both permuter candidates scored IDENTICALLY spliced and raw, which proves there is no header drift behind this one.
- Wave 57: plain non-volatile reads at both sites -- 124 bytes (-4), and the address is cached exactly as it is here. The vu8 casts are a SIZE fix, not evidence that unk2f is volatile: volatile splits value CSE, not address CSE. gcc never needs the base again once base + 0x2f is live in a register, so the reload only appears if the ADDRESS expression is unavailable too.
- Wave 57: (u16)/no-op width casts as a way to break the CSE -- folded away in the front end, zero effect.
- Wave 59 (W59-B): the wave-59 loop-rotation lever does NOT apply -- this function contains no loop, and the residual is a CSE across a call rather than a hoist.
- Wave 79 (W79-C), SIX MORE SPELLINGS by compile_probe, every one of which still parks base + 0x2f in callee-saved r5 and reads the post-call value with a bare ldrb r4,[r5]: (1) all four reads plain and non-volatile (this is the -4, 124-byte shape the wave-57 row describes); (2) volatile on the GUARD read only (the current draft); (3) volatile on the POST-CALL read only, which gives ldrb r0,[r5] then adds r4,r0,#0, so +2 and not +4; (4) the whole case 1/2 arm restructured from one && chain into four early-exit if (...) break; statements, which changes the CFG and changes not one byte; (5) the guard-s unk2f read bound to a u8 w local declared in the case block, which only moves the read above the unk2d test; (6) ((volatile struct Unk03003FC0 *)&gUnknown_03003FC0)->unk2f at the post-call site. The CSE of the address survives all six.
- Wave 79 (W79-C): the pointer-global reading of the pool word is ALREADY REFUTED IN THE TREE and must not be re-derived. The wave-43 W43-A chapter of docs/agbcc-codegen.md measured 0x08090E34 / 0x08090E38 / 0x08090E3C specifically, states the force-addr reading is right for all three, that declaring the word a real T * emits a THIRD load, and that the ROM re-reading the word after every call is normal .LC behaviour rather than evidence of a mutable pointer variable. gUnknown_08090E3C -> 0x03003FC0 is checked against baserom.gba there. The candidate-s own .rodata word is the same mechanism, and the relocation difference is not the residual.

### Settled

- Wave 59 (W59-B): THE 'FREE PAIR' PREMISE IS REFUTED, AND THIS IS THE MEASUREMENT. The wave-59 brief paired this function with its callee sub_08035080 on the grounds that the caller consumes the result as lsls r0,#0x18 / cmp r0,#0, which pins the callee's return to a byte-wide bool, so 'match sub_08035080 first and the caller's contract is fixed for you'. The TYPE claim is correct -- u8 sub_08035080(void) is what matched -- but it is not load-bearing, because the draft already declared it that way and the caller was already size-exact with the correct call sequence before the callee matched. sub_08035080 MATCHED in wave 59 and this caller measured 50.8% with its first difference at +0xc both before (wave 57) and after (wave 59): the callee's status changed not one byte. A pinned callee contract does not make a caller cheap when the caller's residual is in its own body.
- Wave 59 (W59-B): the callee's contract is confirmed unchanged for any future attempt -- u8 sub_08035080(void), no arguments.

### Notes

Wave 79 (W79-C) -- RESIDUAL RECLASSIFIED. The whole 63-byte diff reduces to ONE decision and it is a CSE BASE CHOICE, the wave-77 -does not convert- class: the candidate has ONE pseudo for &gUnknown_03003FC0.unk2f used on both sides of the sub_08035080 call, the ROM has TWO. Everything else follows from that -- because the ROM re-derives post-call it needs the .LC word-s address after a call, so that address takes a callee-saved register and pays the adds r5,r1,#0 at +0xc, which is the first differing instruction. The post-call block is DOMINATED by the pre-call read and both name the same member at the same offset, so the two address computations are unconditionally equivalent and CSE is entitled to unify them; no C text distinguishes them. Fifteen spellings across waves 45/51/57/59/79 have failed. The old wave-59 NEXT LEVER line is WITHDRAWN -- it restated the diff rather than naming a lever. Reminder that has now caught two waves: the DRAFT scores 50.8%, the 85.2% trymatch prints is best.c-s, and best.c reads an UNINITIALISED pointer on the case 1/2 path and must never be promoted. OLD NOTE FOLLOWS. Wave 59 (W59-B). NEXT LEVER: something that makes the tail's address expression re-expand from the .LC word rather than reuse the pre-call pseudo. The address is referenced twice across one call and gcc keeps it; the ROM does not. This is a register-allocation/GCSE question, not a shape question. Binding the struct address to a local is what moves the permuter's score, which is the strongest lead, but the only shape found that way is semantically indefensible.

### Wave 86

WAVE 86 (W86-G): the INLINE siblings of the wave-79 statement-split (three variants in one compile_probe: a top-bound `struct Unk03003FC0 *p` used for the entry and pre-call reads with the plain global elsewhere; `p->` for EVERY reference; a FRESH pointer local bound after the call for the post-call read) all keep `base + 0x2f` in the callee-saved register across the call -- eighteen spellings now across waves 45/51/57/59/79/86. `p->` everywhere is a wrong mechanism outright (collapses the pool word to ONE level, `.word gUnknown_03003FC0` inline, no .LC indirection). MECHANISM NAMED: on the ROM side this is a cse-EBB story -- `_0803518C` (case 0) and `_080351D8` (the bl block) are both JOINS, so cse's table is empty and the ROM re-derives the access from the .LC word after the call; the candidate reproduces the case-0 reload but NOT the post-call one, and cse is not why: at `_080351D8` the candidate's `base + 0x2f` is a live PSEUDO the RTL already names, and unifying the two reads across the join and the call is GCSE (global, on at -O2), which no EBB boundary constrains -- which is why every statement-level respelling (split, inline, fresh local) is identical. No statement boundary reaches it; the lever, if any, has to stop gcse forming the shared pseudo (a different pseudo on each side of the join). No try_match spent; configured, 128/128 size-exact, 85.2%, unchanged (w86g-start.c).

### Wave 89

WAVE 89 (W89-C then W89-H): *** CLASSIFIED FINAL -- DO NOT PRE-REGISTER ANOTHER SPLITTER FOR THIS FUNCTION. *** Two independent results retire the framing this entry has carried since wave 57. (1) W89-C: wave 88's `static inline` re-read helper is INERT here -- four spellings in one compile_probe, byte-identical to all eighteen previous ones. (2) W89-H ran W89-C's kill-the-CSE diagnostic and it came out NEGATIVE: with the offset made `volatile` so no pass can precompute the address, gcc parks the BASE in the callee-saved register and still never reloads the force-addr word, while the ROM parks `&.LC` -- the pool word's OWN address -- and reloads through it. So splitting the address CSE does not change which value wins the register, and TWENTY-TWO SPELLINGS ACROSS SIX WAVES were measuring a fact that was never the residual. THE REAL WALL IS NAMING: `.LC` is invented by `-fforce-addr` after the front end is gone, so no C expression denotes it. Also corrected: the header says size-exact 128/128, but the CODE is -4 (missing `adds r5,r1,#0` plus a two-instruction tail where the ROM has three, all 4 bytes returned as `.short 0x0000` padding) -- the sub_08045FC8 costume. Owner of the contested pseudo is GCSE, measured: `--profile o1` sets flag_gcse=0 and flag_cse_follow_jumps=0 while keeping -fforce-addr, and under o1 the draft builds the post-call address independently while -O2 reuses the guard's pseudo. Evidence: work/sub_08035170/W89-notes.md.

### Wave 89 permuter blocked

WAVE89 PERMUTER-BLOCKED: this draft currently carries a wave-88-style `static inline` helper written with `__inline__`/`__typeof__`, which PYCPARSER REJECTS -- permute.py reports 'could not score the starting point', which is a SYNTAX ERROR, not a result. So no permuter run on the CURRENT draft has ever actually executed, and any permuter negative recorded against a draft carrying the helper is void. `static inline` plus the explicit struct tag is BYTE-IDENTICAL and parses (W89-F, measured). Re-spell before recording any permuter verdict here.

### Wave 96

Base: `sub_08035170.c` (50.78%, 4 bytes short; padding makes it read size+0). `best.c` (85.16%) is wrong C: `new_var = &gPlaySt`
is assigned only in the `case 0` arm and read in `case 1/2`. Its value: it shows the ROM keeps the pool-word address
(`.rodata` word) in r5 across `sub_08035080` and reloads `gPlaySt` through it, i.e. the post-call read of
`defaultWeather` is a fresh load of the address word, not a reuse of the pre-call field address.
Legal rewrites tried: `st = &gPlaySt` bound before the switch and used everywhere / for some reads: 8.6% -8 (the bind
folds into the constant and the address word is used once, so there is no register to hold). Post-call read spelled
plain while the pre-call one is volatile / the reverse / volatile `randomWeatherOn`: 49.2% -4, 47.7%, 46.1%. None
reproduces the split. Left as the earlier note: the address word cannot be held from C. No permuter run (one slot, spent
on sub_08037A78).

### Wave 97

wave 97 (W97-U)
Base unchanged (50.78%, size-exact). Tried a bind of `&gPlaySt` in the post-call arm only (block-scoped, fresh
variable): 50.78%, identical bytes. `static inline` helper returning the post-call `defaultWeather` (volatile cast kept /
dropped): 50.78% / 49.2% -4, identical to the plain draft. `*(u8 *)((u8 *)&gPlaySt + 0x2f)` post-call: 49.2% -4.
Hypothesis not tried but worth a probe: cse's EBB path limit. The ROM's case-block reuses the prologue's pointer (r2) but
the post-call block reloads through the held word address (r5) -- the shape of a path that was CUT before the post-call
block (block/insn count of the path), not of a mutable pointer. Padding the path with extra blocks would change bytes;
no way found to test it without changing code.

wave 97 (W97-W)
Alias lever (gPlaySt vs gUnknown_03003FC0) tested with spellings.py, 5 variants in `w97w.c` (baseline, alias at post-call read, alias at case 0 guard, alias at case 1/2 pre-call reads, alias at both). Baseline 50.78% size-exact; every alias variant is worse (34.85% +4, 42.19% size-exact but first diff +0x2, 43.38% +8, 28.79% +4). NEGATIVE, mechanism: the ROM has ONE force-addr word (gUnknown_08090E3C) that every access goes through (entry read, case 0 reload via r5 = &word, post-call reload). A second name makes the compiler build a second force-addr word / plain literal, so the pool grows; the ROM's split is between reads through ONE word, not between two names. Draft unchanged.

wave 97 (W97-Z)
Read with the N model (docs/agbcc-codegen.md, last chapter): the ROM's `adds r5,r1,#0` at +0xc is PRE's copy `N = P` held in r5 across the call; the post-call read is `mem/u N` then `+0x2f`. The draft unifies the whole `base + 0x2f` address across the call and holds that instead. No loop here, so the loop-size lever does not apply; no new spelling tried beyond W89-W97 (all respells of the +0x2f site are already recorded above).

</details>
