# sub_0804CA98

0x0804CA98, 416 bytes, THUMB, parked.

Best score so far: 17.6%, -8 bytes (best.c).

## What it does

Steps one entry of the per-side animation table gUnknown_02029A10[a1].entries[a2] while the entry is active: it advances the entry's frame and position, can start a new effect through sub_08056E28, and when the animation reaches its last frame it calls sub_08015328(a3).

## How close it is

Compiles to the right size (416 bytes), but 345 of 416 bytes differ (17.1% identical) because the register assignment differs from the first few instructions on. Everything in the diff follows from one missing force-addr word.

## What is left

Make the compiler use a force-addr word for gUnknown_02029A10 in the stepping block: the original loads that address once, from a word the compiler keeps in read-only data, and holds it in a register for the four accesses there, while the frame check at the end uses a plain pool word. The draft names gUnknown_02029A10 directly but still gets only plain pool words.

## Already tried

- The stepping block's four accesses as byte offsets from `(u8 *)gUnknown_02029A10`: one shared address, but a plain pool word and 12 bytes short (taking an address never triggers force-addr).
- The same byte-offset form for the three accesses at the end as well: 36 bytes short.
- Array form in both the stepping block and the end: 4 bytes short, 17.3% identical.
- Binding the table base to a local: identical output (the compiler substitutes the constant back). Binding an entry pointer: 36 bytes short.
- Other mixes of array and byte-offset forms between the stepping block and the end, and goto or early-return forms of the two guards: the pool words do not change.
- Reading unk20 back without the volatile cast: loses the reload after the store (2 bytes).

## Files

- `sub_0804CA98.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at exact size 416/416, 17.1%, 345 differing bytes, first difference +0xa. Tail array/cast and goto spellings are pool-neutral and do not force gUnknown_02029A10. Residual is the force-address choice and its allocation cascade.

### Wave 95

Base: the old draft (kept as `sub_0804CA98.w95-start.c`, 17.1%, size +0, first difference +0xa). Left as the draft.

Pre-registered hypothesis (force-addr word appears when the address is an address VALUE with a variable term, as in c_0804EB78 / c_0804F3C8): REFUTED for this function. The old draft already had the products-plus-`(u8 *)` spelling in its tail, and this wave's rewrite of the whole guarded region in the exemplar spelling (entry pointer `e = a2*sz + a1*grp + (u8 *)g` at the top, `dst` and the two `gUnknown_08552148` entries in the same spelling, `int`-free u16 reads, group stride bound to a `unsigned short` local, tail also in `(u8 *)` spelling, or with the tail by name) produced size -8 / 17.5% (tail `(u8 *)`) or -12 / 14.9% (tail by name). Inlining the entry as a macro instead of a local is byte-identical to the local. In every one of these the `.rodata` word appears for gUnknown_02029B94 only (that is new: the earlier draft had neither); gUnknown_02029A10 stays a plain literal loaded four times.
So the same source shape as c_0804EB78 does not create the word here. Differences between the two: EB78 has a call and other globals' words before the entry, and its entry is used for reads and stores in ONE straight run; this function tests `unk00` and branches to a tail. Unresolved which of those matters.
The `-da` dumps show three surviving `(set P (symbol_ref .LC0))` after cse, which gcse leaves un-unified; the word for B94 is unified. Not chased further.

Proposed summary:
- does: steps a sprite entry's animation counter and, when it wraps, advances the frame and starts the follow-up effect
- status: size-exact draft, ROM holds gUnknown_02029A10 in one held register from a `.rodata` word for the stepping block and a plain literal for the tail
- left: the missing `.rodata` word for gUnknown_02029A10
- tried: entry pointer / inline macro / by-name array / mixed tail spellings (see above); none creates the word for gUnknown_02029A10, though the exemplar spelling does create it for gUnknown_02029B94

</details>
