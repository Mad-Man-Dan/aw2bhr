# sub_0805D344 — sorts the unit list by movement key (244 bytes)

## Wave 93 (W93-A): no score change (16.39%, size-8), one question answered

**Does a faithful sub_0805D344 match under `-fno-gcse`? No, and not because of a
spelling.** This was the wave's question, because sub_0805D438 next door is
byte-exact under that flag and a flag applies per source FILE.

| profile | score | size | first diff |
|---|---|---|---|
| configured | 16.39% | -8 | +0xf |
| `--cflags-add=-fno-gcse` | 33.20% | -20 | +0xa |

The higher score is the trap the brief warns about. At configured the entire
sort half is byte-exact: the ROM's 12-byte frame, both compiler spills, the
four dead volatile loads, ip/sb/sl holding the inner-loop addresses, the
epilogue. Under `-fno-gcse` that structure is gone — the frame drops to 4 with
no spills at all and the inner swap walks two low-register pointers. The ROM's
register pressure is gcse's own work, so this function was built WITH gcse.

**What that settles for sub_0805D438:** a source file occupies a contiguous
address range. `flag_probe` puts the `-fno-gcse` window at sub_0805D338 ..
sub_0805D438, and sub_0805D344 lies between the other two, so {D338, D438} is
not a possible file and {D338, D344, D438} is ruled out. The only file left is
**sub_0805D438 alone**.

## Measured this wave, all negatives, all at configured

- An explicit `m = n - 2;` local instead of writing `n - 2` in both loop
  headers: 17.21% at size-8, but the first difference moves BACKWARDS to +0xa
  because the frame changes. The ROM's two stack slots hold `n - 2` and `i + 1`
  and are COMPILER spills, not source variables. Do not name either.
- Reusing `n` as the outer sort counter (needs `m`): 28.23% at size+4.
- The park says n's register is the only difference. It is not: the fill loop
  also loads the unit-table pointer AFTER the index arithmetic, adds it
  base-owns-destination, and loads the type byte AFTER arg0's pool load. All
  three are source-reachable and all three REGRESS — a pointer bound to the
  type byte and dereferenced at the call is 12.70%, and computing `id * 12`
  into an int local then adding the volatile-read base is 13.11%. So those
  order differences are downstream of n's register, not independent facts.
