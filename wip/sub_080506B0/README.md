# sub_080506B0

0x080506B0, 680 bytes, THUMB, parked.

Best score so far: 57.8%.

## What it does

Sets up the current sprite object (gUnknown_03001FBC) for one side and slot: flip, palette 8, tile number and priority 3. For two unit types it also turns on alpha blending so the sprite is semi-transparent, then computes the sprite's position and places it.

## How close it is

Compiles to the right size (680 bytes) with the original's frame and 57.8% of bytes identical. What is left: two mask constants live in high registers where the original rebuilds them.

## What is left

Make the compiler load gUnknown_03004580's address through a force-addr word, as it already does here for gUnknown_02029710 and gUnknown_085D6A48. The untried idea: the original adds the column offset to the base before adding the row, so read gUnknown_03004580[side][1] through `(u16 *)((u8 *)gUnknown_03004580 + 2)` indexed by the side.

## Already tried

- Reading gUnknown_03004580[side][1] a second time in the 0x17 test: identical output; the compiler merges the two reads.
- `v` as int, u32 or s16, nested-if or switch for the 0x17/0x11 test, and a separate copy for the compare: pool words unchanged.
- `dx`/`dy` as u16 or int instead of s16: a different negate sequence; s16 is required.
- Plain 2-D subscripts on gUnknown_085D6A48 instead of the row-struct cast: an extra add per read.
- Writing the gUnknown_08553C18 term first in the two sums: moves its whole address chain ahead of the entry pointer.
- Binding the tile-number source to a u16 local: no change.

## Files

- `sub_080506B0.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 improved from 692 to 684 bytes against 680 (+12 to +4 section), 38.7%, first difference +0xa. Binding sidep/slotp and reassigning after sub_08015608 makes four documented pool choices coexist; only the forced gUnknown_03004580 word is still missing. Preserve this improved draft.

### Wave 95

Base: the old draft (kept as `sub_080506B0.w95-start.c`, 38.45%, size +4). Draft now 41.96%, size +4, first difference +0xa (unchanged).

What moved it: the first-region read `gUnknown_03004580[side][1]` respelled as `*(u16 *)((u8 *)gUnknown_03004580 + 2 + side * 16)` (38.5 -> 42.0%). The ROM adds the +2 to the loaded word before adding the row offset, and this spelling reproduces that instruction order. Four equivalent spellings (`&g[side][1]`, `(u8 *)&g[side] + 2`, product-first sum, `((u16 *)((u8 *)g + 2))[side * 8]`) are byte-identical to each other.

Pre-registered hypothesis (word appears when the address is an address VALUE with a variable term): NOT confirmed. The value-with-variable-term spelling above still leaves gUnknown_03004580 a plain literal. Note the ROM's word has a single use in the text (the read in the first region); the later `gUnknown_03004582[..][0]` reads are a separate plain pool word, so the "P needs a second use" reading of the gcse chapter (docs: "The .rodata force-addr word is made by GCSE's PRE") does not by itself explain this word. Not resolved.

Not run: the permuter (cannot create the missing word).

Proposed summary:
- does: sets up the sprite for a unit's tile-marker effect and stores its screen offsets
- status: 42% at +4 bytes; three of the ROM's three `.rodata` words are needed, the draft creates two
- left: the `.rodata` word for gUnknown_03004580 (single use in the first region)
- tried: two-index vs byte-offset spelling of the read (byte-offset is better, no word); four equivalent spellings; the u8-cast form does not create the word

### Wave 96

Base: sub_080506B0.c unchanged (41.96%, size +4, first diff +0xa). Restored after probes.

Classification (pre-registered): the ROM's three cells gUnknown_081360CC/D0/D4 are real .text pool words (`ldr rN,=gUnknown_081360D4; ldr rM,[rN]`), each used once. Same construct as sub_08050FF8 from the other side.

Probes (trymatch, all worse):
- Declare `extern u16 (*const gUnknown_081360D4)[8]` and read `*(u16 *)((u8 *)gUnknown_081360D4 + 2 + side*16)`: the double load appears and the pool word becomes gUnknown_081360D4, but 38.45%, size +4, and `sub sp` grows 12 -> 24: the added live pointer pushes the 0xf and -13 mask constants into hi registers (`mov sl,r3`, `mov r9,r0`) that the ROM rematerialises. The +2 also folds into the ldrh displacement (ROM: `adds r1,#2` on the loaded value first).
- Statement-split (`c1 = (u16 *)gUnknown_081360D4 + 1;` then `(u8 *)c1 + side*16`): 33.7%, size +8.
Mechanism: naming the cell object reproduces the double load but costs a live range; the draft's pressure is already at the limit so the mask constants spill to hi regs. Not resolved. Permuter not run (would need the cell reference to hold).

Proposed summary:
- does: sets up the sprite for a unit's tile-marker effect and stores its screen offsets
- status: 42% at +4 bytes
- left: the ROM reaches the mission-id table through the address cell gUnknown_081360D4; naming the cell adds the double load but the frame grows by 12 bytes
- tried: byte-offset read; four equivalent spellings; cell object read inline (38%) and with the +2 bound first (34%)

### Wave 97

wave 97 (W97-X)
Base: levers.py `5d-140+5a-84` (do { } while (0) around the 8-statement effect block, and the tileNum source `gUnknown_02029710[*sidep].unk00` bound to a u16 local before the store); old draft `sub_080506B0.w97x-start.c`. 41.96% +4 -> 52.06% size-exact (680), wrongc OK. Two permuter links (600 s + 500 s, wrongc OK on each; only changes: a `new_var` copy of `*sidep` for the `unk18` index and a `new_var2 = gUnknown_03004582` table bind) -> 57.79%, size-exact, frame `sub sp #0xC` now equal to the ROM's (it was #0x18 before the first link), first difference +0x47 (was +0xa). Third link: NO-IMPROVEMENT.
The pre-registered hypothesis (a force-addr cell word for gUnknown_03004580 needs naming) was not tested this wave: the size now matches because the 03004580 pool word is one word either way; the remaining difference is the relocation kind of the three cells (`.rodata` word vs the ROM's 081360CC/D0/D4 symbols) plus register allocation.
Proposed summary: status 58% size-exact; left: register allocation from +0x47 on; the three cell words are compiler-made in the draft (`.rodata` relocs) where the ROM names them; tried += do-while around the block, tileNum bound to a local, permuter x3.

wave 97 (W97-Z)
No change (57.79%, size-exact, first diff +0x47). New reading from `-da`: gcse's PRE inserts six `.LC` copies at the end of bb 0 (`PRE/HOIST: end of bb 0 ... expression 0,2,4,37,48,50`), so the draft carries six pointer pseudos across the function. The mask constants 0xf and -13 are then pushed into hi registers (`mov r9,r3`, `mov sl,r0`) that the ROM rematerialises; that is the +0x47 difference. The ROM has three cell words (081360CC/D0/D4) each with one visible use, i.e. the same PRE copy with the other uses folded by cse2 and the copy spilled (see the last chapter of docs/agbcc-codegen.md). Making cse2 fold the later uses needs the reading loads inside cse2's block from the copy, or a loop hoist (loop.c 26-insn limit); this function has no loop, so no lever found. Not tried further.

</details>
