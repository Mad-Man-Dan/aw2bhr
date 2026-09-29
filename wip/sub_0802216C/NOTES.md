## wave 95

Base: sub_0802216C.w95-start.c (39.5%, size -8, `register int base asm("r8")` pin). Draft now has the pin removed (plain `int base;`, permuter-compatible); NOT scored or permuted this wave (permuter slot was used up by sub_0801C01C and sub_08022BB8's hand work). ROM detail read: a5..a8 and a3 are spilled at [sp,#0..#0x10] (five slots), a2 in sl, a4 in sb, 0x400 set in r8 at the start of the a3==0/0x80 arm. Next step: chained permuter run from the unpinned draft (never run on this function).

## wave 96

Base: sub_0802216C.w96-start.c (the unpinned draft, 25.0%, size-12; best.c is the same source with `register int base asm("r8")`, whose 39% is the pin, so it was not used). Final source rewritten in plain style (comment replaced); bytes identical to the spellings below.

Result: 25.00% size-12 -> 97.3% size-exact, first difference +0x8a, 15 bytes differ. NOT matched.

What moved it (in order of effect):
1. The `t + r + 0x402` / `t + r + base` pair. Both sums were spelled `t + r`, so gcse computed it once in the first arm and reused it at the end of the chain (`adds r4,r5,r6` kept live in r4 across the calls). That live value is what forced a2 into a stack slot and pushed 0x400 and a3>>6 into the wrong hi registers. Spelling ONE of the two sums differently (`t + (r + 0x402)`) stops the reuse: 212 differing instruction lines -> 102, frame 0x18 -> 0x14, a2 back in sl. Any non-identical spelling of either sum does the same; commuting alone (`r + t` on both) does not. The wave 65 "temp for 0x400" and the wave 71 pin were chasing a symptom of this reuse.
2. Else arm: `dst[1] = t + (r + 1)`, `dst[0x20] = t + (r + 2)`, `dst[0x21] = t + (r + 3)` (constant added to r inside the parentheses). These reproduce the ROM's `adds r0,r5,#K; adds r0,r6,r0` exactly; the source association picks which register gets the constant first.
3. `a6 + (u16)(t + 0x6C)` in both arms (found by the permuter as `a6 + inline_fn(t)` with `inline int inline_fn(u16 x) { return x + 0x6C; }`). The value is unchanged (t is u16 and the store truncates), the u16 conversion of the sum makes the ROM's order: dst+0x42 address first, then the value, and a6 loaded last. A plain `inline` helper also emitted an out-of-line copy of itself; the cast has neither problem.
4. `dst[1] = r + t + base` (r first) in the first arm.

Left (three places, all in the a3 == 0 / 0x80 arm):
* the ROM computes `(r+1)+t` BEFORE materialising 0x400 in r8 (`adds; adds; movs #0x80; lsls; mov r8`), this draft materialises first. Tried `dst[0] = X + (base = 0x400)`, `(base = 0x400) + X`, and `X + 0x400` with `base = 0x400` after: none moves it (the last one is +5 instructions).
* dst[1]: the ROM sums `adds r0,r6,r5` (r first) and this compiles to `adds r0,r5,r6`; every spelling of `r + t` in that expression, including narrowed forms, gives t first here.
* the 0x402 site: ROM is (r+t) then a pool-loaded 0x402; the only spellings that avoid reusing dst[1]'s sum give (t+0x402)+r. A 10 x 10 joint search of association and `(u16)` placement over the two sites found nothing better.
Untested reading: the operand order in the ROM looks like r has the LOWER pseudo number than t (t is assigned before the call that makes r, so it cannot be a declaration matter; declaration order was byte-neutral again here). A source form that creates r's pseudo before t's, without moving the call, is the open question.
A copy `u16 u = t;` taken after `r` (to give the sums a higher-numbered operand) makes it much worse (raw 116 vs 8): u gets its own register.

Pre-registered "two variables" hypothesis for this park: refuted for the mechanism given (the missing bytes are not copies); the size gap was spill/reload traffic caused by ONE value (`t + r`) shared across blocks, i.e. the opposite, too FEW distinct values.

Proposed summary: does = writes a 2 x 2 block of tilemap entries; status = 97.3% identical, size exact; left = operand order of three sums in the a3 == 0 or 0x80 arm and where the 0x400 constant is loaded; tried = `t + r` spellings, u16 conversions, copy of t, base assignment position.

### wave 96, later update (permuter)
A 600 s chain from the 97.3% file dropped the `base` local: writing the literal `+ 0x400` in the three sums keeps the bytes and the ROM's r8 constant still appears. FINAL: 99.11%, size-exact, first difference +0x140, 4 bytes differ: only the dst[0x21] sum in the a3 == 0 / 0x80 arm (ROM `(r+t)` then pool 0x402; this compiles `(t+0x402)+r`). Every spelling that produces `(r+t)+0x402` is 22 instructions worse, because it is again the same `r + t` that dst[1] computed and gcse reuses it (the ROM does not reuse it). A further 500 s permuter run found nothing. Source is work/sub_0802216C/sub_0802216C.c (plain C, no `base` local, no helper function).

### wave 96, final round (lead's hypothesis: keep `t + r + 0x402`, respell the `+ 0x400` site)
Tested 10 spellings of dst[1] (`t + (r + 0x400)`, `0x400 + t + r`, `r + (t + 0x400)`, `(t + 0x400) + r`, `(r + 0x400) + t`, `(u16)` forms, ...) against `t + r + 0x402` and `r + t + 0x402`. Result: the dst[0x21] site then matches the ROM, but every dst[1] respelling loses the ROM's `(r + t)` then `add r0, r8` and emits `mov r3, r8; adds r0,r5,r3; adds r0,r6,r0` instead (raw diff 70 lines vs 4 for the file kept). Keeping dst[1] as `r + t + 0x400` with `t + r + 0x402` is the sharing case again (22 lines). Bringing back an `int base` local for dst[1] (and 0x20 / 0x00 in any subset) is also the sharing case (24 lines). So the two sites are coupled: the ROM's dst[1] and dst[0x21] both need `(r + t)` first, and no spelling found yields both without gcse reusing it. Kept: 99.11%, 4 bytes.
