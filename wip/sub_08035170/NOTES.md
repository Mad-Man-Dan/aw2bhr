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
