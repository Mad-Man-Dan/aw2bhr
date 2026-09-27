# sub_0801A718

0x0801A718, 132 bytes, THUMB, parked.

Best score so far: 79.5%.

## What it does

Inserts an entry into a linked list kept sorted by a signed 16-bit key, taking a free 12-byte node from the pool at gUnknown_0200C624 (list header gUnknown_030020A8). Returns -1 when the pool is full, otherwise 0.

## How close it is

Right size (132 bytes), 79.5% of bytes in place, up from 68.9%. Computing the list cursor before the new node, and deriving both from one local holding the node pool's address, gets the original's register sharing. What is left starts at the second byte and is register choice: the original keeps the first parameter in r3, this build in r5.

## What is left

Add a symbol for the sentinel node at 0x0200C618 to the linker script (aw2bhr.lds) and use it in the last statement. A test compile with that name gives the original's loads and the right size, but only a full build can confirm it. One small register-copy difference in how `cur` is first set may then remain.

## Already tried

- Reaching the sentinel as `gUnknown_0200C624 - 1` in the last statement: the compiler reuses the array's address and subtracts from it, so the instructions still differ.
- Declaring the sentinel in include/unknown-globals.h without adding it to the linker script: the split build fails with an undefined reference.
- Declaring the ROM pointer word `const`: still one load too many.
- Reading through the ROM pointer word gUnknown_0808E5D0 (the current draft): correct everywhere except the one extra load, because inside a loop the compiler adds its own address word on top.

## Files

- `sub_0801A718.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

79.55% -- 132 bytes, SIZE-EXACT (was 68.94% at wave 93 start)

### What still differs

A MISSING LINKER SYMBOL, not a residual in this function's C. The ROM's third pool word must contain 0x0200C618 -- the list-head sentinel record sitting immediately before the gUnknown_0200C624 node array -- and aw2bhr.lds jumps 0x00C528 straight to 0x00C624, so no symbol exists at that address to name. The draft uses the pointer-object workaround `gUnknown_0808E5D0->unk04`, which costs ONE EXTRA `ldr`: agbcc adds its own force-addr level on top, giving `ldr r0,[r6]; ldr r0,[r0]; ldr r0,[r0,#4]` where the ROM has two loads.

### Why it is close

The shape is solved and verified against the listing, including the double `node->unk04 = cur` on the mid-insert path (the store through `prev` may alias `node`) and the loop's rotation into the middle, which is agbcc's and not source.

### Already ruled out

- `gUnknown_0200C624 - 1` for the tail -- produces no second address constant; agbcc keeps the base in a register and CSE folds the two uses, leaving `ldr; sub #8; ldr`.
- Inventing the symbol in include/unknown-globals.h alone -- fails the split build with `undefined reference` (the wave-32 lesson).
- Wave 58: qualifying the pointer-object workaround `const` (`struct Unk0808E5C8 *const gUnknown_0808E5D0`) -- does NOT remove the force-addr level, still three loads. Wave 42's `u16 *const` row, which reaches the right level count for a scalar in sub_08010EF8, does not generalise to a pointer-to-struct object. No declaration of a ROM word reaches two loads; only naming the RAM object does.

### Settled

- THE FIX IS VERIFIED BY PROBE, not proposed. With `extern struct Unk0808E5C8 gUnknown_0200C618;` declared and the tail written `gUnknown_030020A8.unk04 = gUnknown_0200C618.unk04;`, agbcc emits its own `.rodata` address constant, `ldr r6,=.LC` in the preheader and exactly `ldr r0,[r6]; ldr r0,[r0,#4]` at the tail -- the ROM's two loads. The extra indirection is gone.
- The one-line edit is: aw2bhr.lds, EWRAM section, between the 0x00C528 and 0x00C624 lines, `. = 0x00C618; gUnknown_0200C618 = .;`. NOT applied: aw2bhr.lds is upstream's file, tools/gen_lds.py consumes it, and try_match compiles one unit and cannot gate a link error, so the edit is unverifiable without one full split build.
- A residual of one instruction would remain after the symbol exists, and it is localised: `cur = gUnknown_0200C624 - 1` applies the -0xc to the base pseudo in place (`sub r2,r2,#0xc`) where the ROM copies first (`adds r3,r0,#0; subs r3,#0xc` -- THUMB's `subs rD,#imm8` is two-operand, so the copy is exactly the allocator declining to coalesce cur with the base). `cur = &gUnknown_0200C618` restores the count but substitutes `ldr r0,.LC; ldr r3,[r0]`. One try_match decides it.

### Why it is parked

Wave 58 (W58-A), carried from wave 41. Blocked on a linker symbol at 0x0200C618, not on C. The fix is verified by probe and is one line of aw2bhr.lds plus one declaration; it needs a full split build to confirm, which an agent cannot run.

### Wave 92

WAVE 92 (W92-B): no movement (68.94%, SIZE-EXACT); seven spellings measured negative in two compile runs. THE SENTINEL SYMBOL LANDED AND IT WAS WORTH 10 POINTS: aw2bhr.lds now carries 0x00C618 and the draft's tail reads gUnknown_030020A8.unk04 = gUnknown_0200C618.unk04. Re-measured side by side this wave, the old pointer-word spelling gUnknown_0808E5D0->unk04 is 59.09% at the same size, so the symbol is the fix and the workaround must not come back. GOING THROUGH THE ROM WORDS IS WORSE, AND IT IS MEASURED. 0x0808E5C8/CC/D0 hold 0x0200C618, 0x030020A8 and 0x0200C618, and this function's own pool holds 0x0808E5CC (+0x1C) and 0x0808E5D0 (+0x48), which looks like an invitation to name them. Reading gUnknown_030020A8 through a declared gUnknown_0808E5CC is 13.97% at +4; that plus the sentinel through gUnknown_0808E5D0 is 14.29% at +8; the sentinel alone is 59.09% at size-exact. Declaring one of these words and dereferencing it makes agbcc add its own force-addr level ON TOP, one load and four bytes per word, so wave 58's result for gUnknown_0808E5D0 now generalises to gUnknown_0808E5CC. The three words behave exactly as the address-constant cells agbcc emits for a multi-block reference, which means the candidate's .rodata relocation at +0x1C is the honest spelling and the promotion carries it. THE cur/base SPELLING AXIS IS CLOSED: four more spellings, all BYTE-IDENTICAL to the draft -- base bound to a local with node and cur both derived from it; base bound for node only; cur = &gUnknown_0200C624[-1]; cur = (struct Unk0808E5C8 *)((u8 *)gUnknown_0200C624 - 12). Whether base and cur share a register is decided after the source is gone, so it is not reachable by rebinding or by respelling the subtraction; with the two already recorded the axis is six deep. RESIDUAL: pure register allocation on a size-exact function. 41 of 132 bytes differ and the first difference at +0x2 is a register number -- the ROM keeps the first parameter in r3, the draft in r5. NO PERMUTER RUN HAS EVER BEEN MADE HERE and it is the obvious next step (wave 59's repeated-run recipe); this agent's two run slots were taken by sub_080607E8 and sub_0806F41C.

### Wave 93

- **result:** 68.94% -> 79.55%, size-exact throughout, first difference stays +0x2
- **permuter_REJECTED:** First permuter run on this function (900 s x 4 threads) reported IMPROVED 68.94% -> 83.33%. REJECTED as wrong C and discarded. The mutation moved the only assignment to node to AFTER a return -1, into unreachable code, so on the live path node is never set and is then dereferenced four times (node->unk08, node->unk04 twice, and the stores through prev and cur). Only the first store still landed correctly, because the permuter had respelled it as a full address expression. This is the wave-92 read-before-set class. Output kept as w93-perm1-8333.c.wrongc and best.c.wrongc; best.json regenerated from the restored draft.
- **why_the_wrong_C_scored:** Uninitialised node is not a random register -- agbcc leaves its pseudo holding what the first store's address computation already put in a register, so node and the node-array base SHARE a register, and that sharing is worth 14 points. That identified the real target: the six spellings previously tried all aimed at making cur and the base share, and it is node and the base that must.
- **hand_gain:** Acting on that: cur = gUnknown_0200C624 - 1; moved BEFORE node is computed is 76.52%, and deriving both from a base local as well is 79.55%. The adopted form is plain C -- base = gUnknown_0200C624; cur = base - 1; node = &base[(s16)gUnknown_030020A8.unk00]; then the two stores. cur is pure address arithmetic with no dependency on the stores and nothing between reads it, so hoisting it is equivalent.
- **positions_measured:** base bound AFTER the bare first reference 68.94% and unchanged; cur and prev swapped with cur still last 68.18%; cur moved before node 76.52%; that plus a shared base local 79.55%; cur moved above the unk00 > 0x80 guard 10.29% at +4.
- **negative_corrected:** The recorded negative that binding the base with node and cur both derived from it is byte-identical holds only with node computed FIRST. With cur computed first the same base local is worth a further 3 points. The mechanism is the documented one: of two address constants used the same number of times, the pseudo created first wins the register.
- **residual:** 132/132, 27 of 132 bytes differ, first difference +0x2, the first parameter's register. A pure allocation residual at the exact size, and the permuter has not yet been run on this new base.

</details>
