# sub_080364F4

0x080364F4, 296 bytes, THUMB, parked.

Best score so far: 71.6%.

## What it does

Sets up the players for the current map from its chapter record gUnknown_085C77A0[mapID]: each army's colour, CO and one more per-army byte are copied into gPlaySt. It then starts the map's pre-placed-unit script, or calls sub_080364E0 if there is none.

## How close it is

Compiles to the right size (296 bytes) with 71.6% of bytes identical. Reading the record table through a byte pointer inside the case 1/2 block gives the original's extra address word, and a separate temporary per field offset keeps the offsets from being folded. What is left: the draft computes mapID * 0x5c once and shares it, where the original computes it again.

## What is left

The original loads the record table's base address through a pointer word, keeps it in a register, and adds the field offsets (0x3c, 0x40, 0x44) at run time; the draft's plain array indexing lets the compiler fold those offsets into its address constants and order the bases differently. Find a spelling that keeps the base separate without the extra 4-byte stack slot the probes so far caused.

## Already tried

- Naming the original's two private pointer words (at 0x08090EBC and 0x08090EC0) and adding table/base locals: reproduces the original's extra load step but forces a 4-byte stack frame the original does not have.
- Switching directly on the u8 gameMode member: gives an unsigned comparison where the original's is signed, so the switch uses an int copy.
- Reading the offset-0x44 bytes as `unk44[i].unk00`, the shared struct's declared shape: scales the index by 4, which is wrong; they are read through a byte cast instead.

## Files

- `sub_080364F4.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 280/296 (-16), 9.5%. Private-slot/table-base probes restore the initial indirection but force a non-ROM four-byte frame. This confirms base association/allocation rather than missing statements; shared structs and the readable semantic draft are retained.

### Wave 95

Base: the old draft (kept as `sub_080364F4.w95-start.c`). Result: 9.5% / -16 -> **17.6% / -8**
(`sub_080364F4.w95-v9.c`, draft left as that file). The first difference is still +0xe because
the ROM's `ldr r0,=word; ldr r2,[r0]; ldrb r1,[r2,#1]; cmp r1,#0; beq` opens with the mode test
before the second word is loaded, and the draft loads it in the same place except for register
naming.

What moved it (mechanism):
- The second `.rodata` word (holds gUnknown_085C77A0) appears ONLY when the table address is bound
  through a BYTE cast: `t = (u8 *)gUnknown_085C77A0;` inside the case-1/2 block. Binding a
  `const struct Unk085C77A0 *t = gUnknown_085C77A0` at the top (or `&arr[0]`) emits no word: the
  address stays a folded constant. The tail's bare `gUnknown_085C77A0[...]` then also loads through
  the word, and case 0's bare use keeps the plain `gUnknown_085C77A0+0x3c` literal: the ROM's
  `mov r1,sl; ldr r3,[r1]` tail and `=gUnknown_085C77DC` in arm 0 both come out right.
- The 4-byte frame the wave-71 probe paid does NOT appear: the bind is a plain cast local, no
  volatile, `sub sp` unchanged. (That probe's extra locals were live across the loop.)
- The `+0x44 / +0x40 / +0x3c` offsets stay separate from the index only with a per-statement temp:
  `(d1 = t + 0x44, d1[gPlaySt.mapID * 0x5c + i])`, one temp per offset (d1/d2/d3). Without the temp the
  constant sinks to the end (`adds r0,#0x44` after the sum) even when written `x + (t + 0x44)`.
  One distinct temp per statement transferred from W95-A's lever: yes.

Residual (-8): LICM hoists `t + 0x44` and `t + 0x40` into registers r9/r8 with the ROM computing them
inside the loop (`adds r1,r6,#0; adds r1,#0x44` per use), and only `t + 0x3c` (used twice) is hoisted.
So the ROM's 0x44/0x40 adds are NOT loop-invariant to loop.c: something makes them variant or
un-movable. Tried d-temps declared per block: same. Then the register roles differ (arm 0 uses r3/r4
swapped) as a consequence. Not tried: permuter (never run), a walker for the +0x44 read.

Proposed summary:
- does: (unchanged)
- status: 17.6% at -8 bytes
- left: the ROM adds 0x44 and 0x40 to the table base inside the loop, while the draft hoists them into registers; only 0x3c is shared and hoisted
- tried: table base bound as a pointer-to-struct (no address word emitted); byte-cast bind + per-statement offset temps (word appears, -8); folded byte-index forms (offset sinks to the end)

### wave 95, permuter (three chained 600 s runs, 2 threads, from `sub_080364F4.w95-v9.c`)

17.6% / -8  ->  61.5%  ->  70.6%  ->  71.6%, all **size-exact (296)**. Kept: `sub_080364F4.c`
(= perm3 output with `t - -0x40` written back as `t + 0x40`, `new_var` renamed `isEmpty`; measured 71.6%
before and after). Every kept mutation was read and is semantically the same C:
1. run 1: `(i & 0xFF)` on the index of the +0x44 read (the fold-proof mask: `i` is u8, so no-op) and the
   `== 0xff` test bound to a `u8` temp before the `&& i + 1 <= 3` (that is `isEmpty`). Together: -8 -> size-exact. This is what
   stops LICM hoisting `t + 0x44`.
2. run 2: `mode = mapID * 0x5c + i;` reused as the index of the +0x40 read (`mode` is dead after the switch
   dispatch) and `(&gPlaySt)->mapID` in the tail.
3. run 3: operand swap `i + mapID*0x5c`, and a `do { } while (0)` around the else-arm store.
Residual: the permuter shares the `mapID * 0x5c` product between two reads (`mode`), where the ROM
recomputes the product for every read (`mov r1,ip; ldrb r0,[r1,#2]; muls r0,r5` three times). So 71.6% is
size-exact but structurally the ROM's *other* spelling: next step is to un-share the product (three
distinct one-statement temps) while keeping the `& 0xFF` and the `isEmpty` temp. First difference is +0x10:
the ROM's first `ldr r2,[r0]` uses r2 for the play-state pointer where the draft uses r1.
Proposed summary status: 71.6% at the right size; the `.rodata` words for the play-state pointer and the map table are now both produced.

### Wave 96

Base: `sub_080364F4.w96-start.c` (= wave 95 draft, 71.6%, size-exact, first diff +0x10). Pre-registration (un-share the
`mapID * 0x5c` product) tested and REFUTED as stated: deleting `mode` and writing the product in both reads gives
61.5% (size-exact), worse. Mechanism: the product is already recomputed in the ROM's sense (every `strb` through
`gPlaySt` forces `mapID` to be reloaded, so cse cannot share it); what deleting `mode` changes is loop.c's hoisting:
with both reads written as a plain `base + idx` the compiler hoists `t+0x44`, `t+0x40` and `t+0x3c` into registers
(three invariants, r9/r8/r6), where the ROM hoists only `t+0x3c` and does `adds r1,r6,#0; adds r1,#68` inline per
read. The `(i & 0xFF)` mask on the 0x44 read does NOT stop that hoist on its own (with `mode` gone); it is `mode`
feeding the 0x40 read that keeps a hoistable invariant out. Other spellings measured: folding the constant into the
index (`t[0x44 + ...]`, `(t+0x44)[...]`): 14.0% +4 (offset folds away entirely); a fresh `mode = i + mapID*0x5c;`
statement before each read: 25% +4; before only read 1 or only read 2: 28-29% -8.
Residual unchanged (71.6%): loop invariant set differs (ROM: only +0x3c hoisted).

### Wave 97

wave 97 (W97-U)
Base `sub_080364F4.c` (71.62%, size-exact). Pre-registration (lever 5 / lever 1 on the offsets) not confirmed as an
independent lever: the offsets already come out as the ROM has them (only +0x3c hoisted into r8). Probed: a u8 `nx = i + 1`
computed at the top of the body and used for every `[i + 1]` and the `<= 3` test, with `i = nx` as the loop step: 27.0%
size-exact, first diff +0xE; with `i++` kept: 19.6% -4; without the `isEmpty` temp (plain `==0xff &&` test): 23.0% -8 /
10.1% -12. The ROM's `i+1` sits in r3 computed before the first store address, so it is a real early value, but making
it a variable moves the frame allocation. Permuter (600 s, 2 threads, from the wave-95 base): NO-IMPROVEMENT (best
intermediate scores 4380 vs 4660 were not verified size-exact). Residual unchanged: register roles at +0x10 (gPlaySt
pointer r2 in the ROM, r1 here; mode r1 vs r3).

</details>
