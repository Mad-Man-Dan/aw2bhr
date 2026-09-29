## wave 96

Base: `sub_08035170.c` (50.78%, 4 bytes short; padding makes it read size+0). `best.c` (85.16%) is wrong C: `new_var = &gPlaySt`
is assigned only in the `case 0` arm and read in `case 1/2`. Its value: it shows the ROM keeps the pool-word address
(`.rodata` word) in r5 across `sub_08035080` and reloads `gPlaySt` through it, i.e. the post-call read of
`defaultWeather` is a fresh load of the address word, not a reuse of the pre-call field address.
Legal rewrites tried: `st = &gPlaySt` bound before the switch and used everywhere / for some reads: 8.6% -8 (the bind
folds into the constant and the address word is used once, so there is no register to hold). Post-call read spelled
plain while the pre-call one is volatile / the reverse / volatile `randomWeatherOn`: 49.2% -4, 47.7%, 46.1%. None
reproduces the split. Left as the earlier note: the address word cannot be held from C. No permuter run (one slot, spent
on sub_08037A78).

## wave 97 (W97-U)

Base unchanged (50.78%, size-exact). Tried a bind of `&gPlaySt` in the post-call arm only (block-scoped, fresh
variable): 50.78%, identical bytes. `static inline` helper returning the post-call `defaultWeather` (volatile cast kept /
dropped): 50.78% / 49.2% -4, identical to the plain draft. `*(u8 *)((u8 *)&gPlaySt + 0x2f)` post-call: 49.2% -4.
Hypothesis not tried but worth a probe: cse's EBB path limit. The ROM's case-block reuses the prologue's pointer (r2) but
the post-call block reloads through the held word address (r5) -- the shape of a path that was CUT before the post-call
block (block/insn count of the path), not of a mutable pointer. Padding the path with extra blocks would change bytes;
no way found to test it without changing code.

## wave 97 (W97-W)
Alias lever (gPlaySt vs gUnknown_03003FC0) tested with spellings.py, 5 variants in `w97w.c` (baseline, alias at post-call read, alias at case 0 guard, alias at case 1/2 pre-call reads, alias at both). Baseline 50.78% size-exact; every alias variant is worse (34.85% +4, 42.19% size-exact but first diff +0x2, 43.38% +8, 28.79% +4). NEGATIVE, mechanism: the ROM has ONE force-addr word (gUnknown_08090E3C) that every access goes through (entry read, case 0 reload via r5 = &word, post-call reload). A second name makes the compiler build a second force-addr word / plain literal, so the pool grows; the ROM's split is between reads through ONE word, not between two names. Draft unchanged.

## wave 97 (W97-Z)
Read with the N model (docs/agbcc-codegen.md, last chapter): the ROM's `adds r5,r1,#0` at +0xc is PRE's copy `N = P` held in r5 across the call; the post-call read is `mem/u N` then `+0x2f`. The draft unifies the whole `base + 0x2f` address across the call and holds that instead. No loop here, so the loop-size lever does not apply; no new spelling tried beyond W89-W97 (all respells of the +0x2f site are already recorded above).
