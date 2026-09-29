
## wave 96

Base: `sub_0803A2BC.w96-start.c` (23.4%, -4). The four missing bytes are two register copies in the prologue. The ROM
keeps the incoming u8 in its own pseudo (`lsls/lsrs` into r1), copies it to r4, adds 8, copies the sum to r0, and shifts
into r4: sum and shifted column are separate pseudos and the widened argument is a third. The draft widens in place in
r4 and shifts in place. Also a2 lands in r6 in the ROM and r7 in the draft (r7 holds 0x8000 there).
Tried, every one byte-identical to the draft (23.39%): locals for `x = a1`, `sum`, `col` in every split
(u32, int, u8 temp; sum and col on separate statements; `>>=`); the shift spelled `/ 8u`, `(u32)(a1+8) >> 3`,
`((u32)a1 + 8) >> 3`. Single-block locals collapse in cse/flow before local_alloc, so the copies are not reachable this way.
Declaring the parameter `int` conflicts with the header's `u8` prototype. Not tried: permuter.

## wave 97 (W97-U)

Base: the 23.39% -4 draft. Hand probes (spellings.py): `i = (a1+8u)>>3` as call argument, `(i = a1+8u)>>3` at both calls,
`a1 - -8u`, `>> 3u`, `/ 8`, `i = a1 + 8u; i >>= 3;`, `(u32)(a1+8) >> 3`, `i = a1;` then `(i+8u)>>3`: ALL byte-identical to
the draft (120 B, 23.39%; `>> 3u` 22.6%). One 500 s permuter run: 23.39 -> 44.35%, size-exact 124, kept as
`_w97-longlong-44pct.c`: `unsigned long long new_var = (a1 + 8u) >> 3;` passed to both calls. wrongc: same behaviour on 400
seeds, so the value is right, but it is a size PAD, not the ROM's structure: the extra pseudo is a 64-bit one, the
constant 0 is no longer held in r5 across the calls (two `movs r0,#0`) and a2 is still r7 not r6. NOT adopted; the draft
stays the clean 23.39% form. Reading the ROM head: `adds r4,r1,#8` is not encodable (imm3), so the `adds r4,r1,#0;
adds r4,#8` pair is natural once the zero-extended argument sits in r1 and the sum in r4 -- the missing thing is only that
the parameter pseudo must NOT be the one that lives across the calls (in the ROM the widened argument dies in r1; the sum
and the shifted value live in r4/r0). Any spelling where one pseudo carries a1 across the calls (all of ours) coalesces.
