
## wave 96

Base: `sub_0803A2BC.w96-start.c` (23.4%, -4). The four missing bytes are two register copies in the prologue. The ROM
keeps the incoming u8 in its own pseudo (`lsls/lsrs` into r1), copies it to r4, adds 8, copies the sum to r0, and shifts
into r4: sum and shifted column are separate pseudos and the widened argument is a third. The draft widens in place in
r4 and shifts in place. Also a2 lands in r6 in the ROM and r7 in the draft (r7 holds 0x8000 there).
Tried, every one byte-identical to the draft (23.39%): locals for `x = a1`, `sum`, `col` in every split
(u32, int, u8 temp; sum and col on separate statements; `>>=`); the shift spelled `/ 8u`, `(u32)(a1+8) >> 3`,
`((u32)a1 + 8) >> 3`. Single-block locals collapse in cse/flow before local_alloc, so the copies are not reachable this way.
Declaring the parameter `int` conflicts with the header's `u8` prototype. Not tried: permuter.
