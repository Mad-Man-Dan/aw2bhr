
## wave 95

Base: draft (lnkp helper, 1388, 20.75%), kept as `sub_0802FACC.w95-start.c`; draft unchanged. best.c / recovered.c (29%) carry `new_var` and pasted headers and were not adopted.
Lever 2 (bind the global's address to a local, leave first reference bare; mixed form) did NOT transfer. Every variant that used a local `struct Unk0849B018 **lp = &gUnknown_0849B018` for some of the reads got SMALLER, not the wanted pool word plus shared base:
- first read bare, lp for the rest: 1288 (-100), 8.1%
- lp bound before everything: 1284 (-104)
- lnkp() first half, lp from the `unk01 == 1` block on: 1300 (-88)
- lnkp() first half, lp for the tail block: 1356 (-32)
- lp for the head, bare global in the tail: 1356 (-32)
Mechanism: a local holding the ADDRESS of a pointer global lets cse/load-combining share the reads of the pointer through one register across blocks, deleting reloads (30 to 100 bytes) that the ROM keeps. In sub_08068038 the bound thing is a table BASE that the ROM's code indexes; here it is a pointer global that the ROM re-reads after each call, so the bound local removes needed reloads. The lever does not apply to a pointer global re-read across calls.
