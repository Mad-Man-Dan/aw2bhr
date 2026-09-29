
## wave 95

Base: the old draft (kept as `sub_0804BB74.w95-start.c`, 29.5%, size +12). Left as the draft.

Pre-registered hypothesis (an address VALUE with a variable term makes the missing word / single value appear): NOT confirmed. Respelling both loops' reads as `*(u16 *)(j * 64 + i * 2 + (u8 *)gUnknown_0200FC50)` / `(k * 2 + (u8 *)...)` is 28.6% at +12, i.e. the same two values as the array-cast form (the plain reload of gUnknown_0200FC50 in each loop stays).

The residual differs from the recorded one in one detail worth keeping: in the ROM the held values are gUnknown_08555850 in r8 (used again for the `unk02` test after the decompress call) AND gUnknown_0200FC50 in r7 (the loop base), the latter loaded once, AFTER the first decompress argument is computed. Binding `src = (u16 *)gUnknown_0200FC50` as the decompress argument (`LZ77UnCompWram(g[a].unk08, src = ...)`) gives the ONE 0200FC50 value (size -8, 11.1% -- worse score, closer shape) but then the allocator swaps r7/r8 (08555850 wins r7) and blockA/blockB gain a plain `gUnknown_085519FC` pool word next to the `.rodata` one, so the size goes the other way. The same bind after the call (`src` as its own statement first) is -4 / 18.5%. Kept as `sub_0804BB74.w95-src.c`.
So the "TWO values" residual is real and the bind removes it, but it exposes the allocation-order question (which of 08555850 / 0200FC50 gets the lower callee-saved register) plus the plain 085519FC word; neither has a lever yet. Not run: permuter (structure is not size-exact on any variant).

Proposed summary:
- does: decompresses a tile map, then copies it into the map buffer with a per-tile offset, using one of three loops chosen by the map kind, and finishes with a fast copy
- status: 29.5% at +12 bytes
- left: the ROM holds gUnknown_0200FC50 as ONE value (loaded after the first decompress argument) and 08555850 in r8; the draft holds 0200FC50 as two values
- tried: single `src` bind (removes the second value, swaps r7/r8 and adds a plain 085519FC word), byte-offset `(u8 *)` sum spelling (no change)
