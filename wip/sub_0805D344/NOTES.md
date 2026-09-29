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

## wave 95

Base: draft (236, -8, 16.4%) kept as `sub_0805D344.w95-start.c`. best.c form (n reused as outer counter with `m = n - 2`) gave 28.2% at +4; the `for (n = 0; m >= n; ...)` spelling of it reached 33.9% at +4 but folds `m >= 0` into a branch the ROM does not have, so it was dropped. Copying n to a second variable by hand (`k = n; m = k - 2`) is byte-identical (copy propagates).
RESULT: SIZE-EXACT (244), 75.0%, first difference +0xf. Draft = `sub_0805D344.w95-perm3-start.c` = current `sub_0805D344.c`. Chained permuter: run 1 (600 s) 16.4 -> 72.1; run 2 72.1 -> 75.0; run 3 75.0 -> 80.7 was WRONG C (`n = n > 1; new_var = n;` clobbers the list length; kept as `.w95-WRONG-80.c`); run 4 75.0 -> 75.8 was `volatile unsigned a1` (parameter made volatile; kept as `.w95-volatile-a1-75_82.c`, not adopted).
What the two kept steps are (checked by reading, semantics identical to the start):
1. Lever 2 transfers: the address of the unit-table pointer is bound to a local once at the top (`new_var2 = &gUnknown_08499594;`) and the volatile-cast read goes through it. That took 16.4 -> 72.1 and made the size exact (the old draft was 8 short).
2. Lever 1 transfers in the form "copy the list length into a per-block variable for the sort" (`new_var3 = n; ... i <= new_var3 - 2 ... j = new_var3 - 2`): 72.1 -> 75.0.
Residual: n is still in a LOW register (r5) with a hoisted-address difference in the fill loop (`ldr r2,[r6]` before the index arithmetic; ROM loads the table pointer after). The ROM's n lives in r8. The 80.7% form got n into a high register only by destroying it, so a high register for n is reachable only if a second variable, not n, takes the flag / copy role.
Pool words: none new.
Proposed summary: status=size-exact, 75% identical, only n's register and the order of three loads in the fill loop differ; tried += "binding &gUnknown_08499594 to a local at the top with the volatile read through it: size-exact (kept)"; "per-block copy of n for the sort loops (kept)". Rename new_var2 -> unitTable, new_var3 -> count when promoting, re-checking bytes.
