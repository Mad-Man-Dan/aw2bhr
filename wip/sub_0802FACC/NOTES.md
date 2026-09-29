
## wave 95

Base: draft (lnkp helper, 1388, 20.75%), kept as `sub_0802FACC.w95-start.c`; draft unchanged. best.c / recovered.c (29%) carry `new_var` and pasted headers and were not adopted.
Lever 2 (bind the global's address to a local, leave first reference bare; mixed form) did NOT transfer. Every variant that used a local `struct Unk0849B018 **lp = &gUnknown_0849B018` for some of the reads got SMALLER, not the wanted pool word plus shared base:
- first read bare, lp for the rest: 1288 (-100), 8.1%
- lp bound before everything: 1284 (-104)
- lnkp() first half, lp from the `unk01 == 1` block on: 1300 (-88)
- lnkp() first half, lp for the tail block: 1356 (-32)
- lp for the head, bare global in the tail: 1356 (-32)
Mechanism: a local holding the ADDRESS of a pointer global lets cse/load-combining share the reads of the pointer through one register across blocks, deleting reloads (30 to 100 bytes) that the ROM keeps. In sub_08068038 the bound thing is a table BASE that the ROM's code indexes; here it is a pointer global that the ROM re-reads after each call, so the bound local removes needed reloads. The lever does not apply to a pointer global re-read across calls.

## wave 96

Base: current draft (all-`lnkp()`, size-exact, 20.75%), kept as `sub_0802FACC.w96-start.c`; draft unchanged.

Pre-registered hypothesis (the ROM's 18 extra register copies) HELD, and the values are named: they are 17-18 copies of ONE value,
the address of the compiler's `.rodata` force-addr cell for gUnknown_0849B018, which the ROM keeps in r9 and re-copies
(`mov r1/r2/r0, sb`) before each `ldr rX,[cell]; ldr rY,[rX]` re-derivation of the record. The `lnkp()` draft has no cell at all
(plain `.word gUnknown_0849B018` pool entries), so it re-LOADS the address from the pool where the ROM COPIES it from r9: that is
the "same size, 18 fewer copies" trade. Measured: `w89f-allbare.c` (every reference bare) has 18 `mov rX, r9` copies, the same
count as the ROM, but is 1384 bytes and saves r8, r9 and sl. So the copy count and the save set are the two halves of one trade
and the residual is unchanged from wave 89: the bare spelling gets the cell and the copies, the helper spelling gets the save set.
Not resolved; the untried step is a permuter chain from the all-bare file (only the `lnkp()` file has ever been permuted).

Proposed summary: does = link state machine, one call per frame; status = size-exact, wrong register set for the record address;
left = the ROM keeps the force-addr cell address in r9 and re-derives the record from it at every reference, the helper spelling
drops the cell; tried = all-bare (cell + copies, but a third saved register and 4 bytes short), all-helper (size and save set, no cell).
