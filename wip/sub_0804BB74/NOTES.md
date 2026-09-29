
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

## wave 96

Base: the wave-95 draft (29.46%, size +12, first diff +0x10) restored; probes below all lose to it on score, and the closer-shape variant stays `sub_0804BB74.w95-src.c`.

Reading of the ROM: gUnknown_08555850 is a plain pool word loaded FIRST into r8 (`ldr r1,=; mov r8,r1`), and the row address is built at run time (`adds r0,#8; adds r0,r5,r0` = base + 8, then + a*24); gUnknown_0200FC50 is loaded second into r7 and used as both the decompress destination and the loop base. So the ROM's pool has 08555850 (not 08555858) and the +8 is a run-time add on a held register. Classification for the copy screen: not the two-variables lever and not one hi-register copied down; here it is register ORDER (which of two long-lived addresses gets r7 vs r8) plus a constant-fold that the ROM did not do.
Probe: `w95-src` plus `*(void **)((u8 *)gUnknown_08555850 + 8 + a * 24)` for the decompress source: byte-identical result to w95-src (316 bytes, -8, 11.1%). agbcc folds the +8 into the symbol (`.word gUnknown_08555850+8`-style single literal, then `subs r0,#8` for the unk02 read), so the base is never a held register. The way to keep base and +8 apart is the same as elsewhere: something that makes the base a non-constant pseudo, which no plain spelling did.
Not run: permuter (no size-exact variant).

Proposed summary: as the wave-95 entry; add "the ROM keeps 08555850 as an unfolded base in r8 and adds 8 at run time; every spelling tried lets the compiler fold the 8 into the literal".
