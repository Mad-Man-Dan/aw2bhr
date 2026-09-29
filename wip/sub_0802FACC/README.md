# sub_0802FACC

0x0802FACC, 1388 bytes, THUMB, parked.

Best score so far: 29.0% (best.c).

## What it does

The per-frame update for link-cable play. It checks for timeouts and lost connections, reads and handles one packet from each player's receive ring (sub_0802F6A0), and sends the next queued packet.

## How close it is

Never parked (status `asm`). The draft compiles to the right size (1388 bytes) and saves the same registers on entry as the original, but only 20.7% of bytes are identical: the original reaches the link record gUnknown_0849B018 through an extra compiler-made address word and ours does not, which changes registers from the first instructions on.

## What is left

The original has both one shared base register for the record and the compiler's extra address word; every spelling so far gets only one. Naming the global directly gives the word but an extra saved register, and the `lnkp()` helper gives the single base but no word. Find a spelling that gives both, or keep chaining permuter runs from the current draft: they reached 31.05% at the right size, but that best.c has the headers pasted in and cannot be promoted as is.

## Already tried

- Naming gUnknown_0849B018 directly everywhere: 4 bytes short and saves one register too many.
- Using a helper only in some places, mixed with direct names: right size, but the extra saved register comes back. A helper has to be used at every reference.
- Putting one direct reference back into the all-helper draft: no change. The address word appears only when the global is named many times.
- volatile casts on the reads after the calls, or a memory barrier around the call results: no useful change.
- Pinning the loop counter i to a register: right size, but the generated code is wrong (the compiler reuses that register while i is still needed).
- Writing out the `next = i + 1` updates in each case-4 arm: 52 bytes too long.
- A union for the field at offset 2: the compiler aligns the union to 4 bytes, which moves the later fields.
- Returning directly instead of jumping to the shared abort tail: costs a register.

## Files

- `sub_0802FACC.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

Worked across several waves without a match; the full record is the draft's header comment and the notes files in work/sub_0802FACC/.

### Wave 95

Base: draft (lnkp helper, 1388, 20.75%), kept as `sub_0802FACC.w95-start.c`; draft unchanged. best.c / recovered.c (29%) carry `new_var` and pasted headers and were not adopted.
Lever 2 (bind the global's address to a local, leave first reference bare; mixed form) did NOT transfer. Every variant that used a local `struct Unk0849B018 **lp = &gUnknown_0849B018` for some of the reads got SMALLER, not the wanted pool word plus shared base:
- first read bare, lp for the rest: 1288 (-100), 8.1%
- lp bound before everything: 1284 (-104)
- lnkp() first half, lp from the `unk01 == 1` block on: 1300 (-88)
- lnkp() first half, lp for the tail block: 1356 (-32)
- lp for the head, bare global in the tail: 1356 (-32)
Mechanism: a local holding the ADDRESS of a pointer global lets cse/load-combining share the reads of the pointer through one register across blocks, deleting reloads (30 to 100 bytes) that the ROM keeps. In sub_08068038 the bound thing is a table BASE that the ROM's code indexes; here it is a pointer global that the ROM re-reads after each call, so the bound local removes needed reloads. The lever does not apply to a pointer global re-read across calls.

### Wave 96

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

</details>
