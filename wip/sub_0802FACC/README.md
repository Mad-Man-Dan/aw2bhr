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
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

Worked across several waves without a match; the full record is the draft's header comment and the notes files in work/sub_0802FACC/.

</details>
