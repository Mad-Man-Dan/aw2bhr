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

## wave 96

Findings: (1) `best.c` (80.74%) is the known-wrong `n = n > 1; new_var = n;` form (byte-identical to `.w95-WRONG-80.c`); the drafts.py
"better" hint is a trap. (2) The 75.00% size-exact draft is size-exact only by accident: replacing its `new_var = n > 1; if (new_var)` with
plain `if (n > 1)` drops it to 236 bytes (-8, 18.4%). The 8 missing bytes are exactly n's four hi-register moves (+2 each), so n living in
r8 (ROM) is the whole real residual; the flag temp was filling the gap with junk (`movs r0,#0 / cmp / movs r0,#1 / cmp r0,#0 / beq`).
(3) Permuter run from the honest -8 form (900 s, 2 threads, `perm-w96-1.log`): 16.39 -> 70.90, size-exact, first +0xf. The kept change is
`long long new_var = n;` used as the index in the fill loop: valid C, same meaning, but it buys the 8 bytes with a 64-bit pair, not n in r8.
Saved as `sub_0805D344.w96-perm1-longlong.c`, NOT adopted. Draft `sub_0805D344.c` = the honest pure form (`.w96-perm1-start.c`, 236 bytes, -8).
Not reached: why r6/r7 are unavailable to n in the fill loop (ROM keeps ptr in r5, walker r4, n in r8). NEXT: `tools/rtldump.py` .greg conflicts for n.
Proposed summary: left: n in r5 where ROM keeps it in r8 (8 bytes); tried += flag-temp size padding is not real, long long index temp.

## wave 97 (W97-V)
Base: w87 draft (16.39% -8). levers 5a-55 = bind `n > 1` to a local (`big = n > 1; if (big)`); wrongc OK (400 seeds). try_match: 70.49% size-exact 244 B, first diff +0xf (unchanged: `n` in r5 not r8). The gain is SIZE only: the flag costs the 8 bytes that n-in-r5 saves (movs #1/cmp/beq), so it is padding, not the ROM's mechanism. Round-2 levers on the new base: nothing above 70.49%. `last = n - 2` bind changes frame (sub sp #8), `n >= 2` no change. Residual still n's register (r8). Kept the flag draft at sub_0805D344.c; pre-lever draft is sub_0805D344.w97v-start.c.
