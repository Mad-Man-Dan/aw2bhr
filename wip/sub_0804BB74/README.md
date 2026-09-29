# sub_0804BB74

0x0804BB74, 324 bytes, THUMB, parked.

Best score so far: 29.5%, +12 bytes.

## What it does

Decompresses picture `a` and turns it into BG tilemap entries, then copies them to `dst`. With `d` set the picture is drawn mirrored (flip bit and palette 4), otherwise straight with palette 1. What the pictures are is unknown.

## How close it is

Compiles 12 bytes too long (336 against 324). 225 of 324 bytes differ (30.6% identical), but that score means little because the size difference starts near the top and shifts everything after it. The extra bytes come from gUnknown_0200FC50's address being held as two separate values (one for the decompress call, one for the loops) and from one extra pool word for gUnknown_085519FC in the 17-column loop.

## What is left

Find a spelling where one value holds gUnknown_0200FC50's address for the decompress call, both mirrored loops and the final copy, while each loop still reloads the gUnknown_085519FC pointer from memory on every step, as the original does. Every spelling tried so far gets one of these two but not both.

## Already tried

- A `src` local for gUnknown_0200FC50 assigned before the decompress call and passed to it: one shared address, but the per-step pointer reload is lost in the 32-column loop.
- The same local assigned after the call: the reload survives, but `dst` is spilled to the stack (4 more bytes of frame).
- Binding `u16 **p = &gUnknown_085519FC` in each arm: 4 bytes shorter, but the pointer is loaded once before the loop instead of every step and `dst` is spilled (13.9% identical).
- Naming gUnknown_08136044 (the read-only word holding gUnknown_085519FC's address) directly, in place or through a local: an extra indirection, or the pointer is loaded once before the loop.
- The 17-column loop as a pointer walk (`row += 32`): removes the row counter as the original does, but adds a loop-entry check the original does not have.
- The range test as a switch or a signed compare: wrong branch instruction; `(u32)(u8)(unk02 - 1) <= 1` is required.

## Files

- `sub_0804BB74.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 336/324 (+12), 30.6%, first difference +0x10. The established semantic draft was reverified; prior pointer-global spelling axes remain exhausted. Residual is address allocation and excess code.

### Wave 95

Base: the old draft (kept as `sub_0804BB74.w95-start.c`, 29.5%, size +12). Left as the draft.

Pre-registered hypothesis (an address VALUE with a variable term makes the missing word / single value appear): NOT confirmed. Respelling both loops' reads as `*(u16 *)(j * 64 + i * 2 + (u8 *)gUnknown_0200FC50)` / `(k * 2 + (u8 *)...)` is 28.6% at +12, i.e. the same two values as the array-cast form (the plain reload of gUnknown_0200FC50 in each loop stays).

The residual differs from the recorded one in one detail worth keeping: in the ROM the held values are gUnknown_08555850 in r8 (used again for the `unk02` test after the decompress call) AND gUnknown_0200FC50 in r7 (the loop base), the latter loaded once, AFTER the first decompress argument is computed. Binding `src = (u16 *)gUnknown_0200FC50` as the decompress argument (`LZ77UnCompWram(g[a].unk08, src = ...)`) gives the ONE 0200FC50 value (size -8, 11.1% -- worse score, closer shape) but then the allocator swaps r7/r8 (08555850 wins r7) and blockA/blockB gain a plain `gUnknown_085519FC` pool word next to the `.rodata` one, so the size goes the other way. The same bind after the call (`src` as its own statement first) is -4 / 18.5%. Kept as `sub_0804BB74.w95-src.c`.
So the "TWO values" residual is real and the bind removes it, but it exposes the allocation-order question (which of 08555850 / 0200FC50 gets the lower callee-saved register) plus the plain 085519FC word; neither has a lever yet. Not run: permuter (structure is not size-exact on any variant).

Proposed summary:
- does: decompresses a tile map, then copies it into the map buffer with a per-tile offset, using one of three loops chosen by the map kind, and finishes with a fast copy
- status: 29.5% at +12 bytes
- left: the ROM holds gUnknown_0200FC50 as ONE value (loaded after the first decompress argument) and 08555850 in r8; the draft holds 0200FC50 as two values
- tried: single `src` bind (removes the second value, swaps r7/r8 and adds a plain 085519FC word), byte-offset `(u8 *)` sum spelling (no change)

</details>
