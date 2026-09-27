# sub_080303C8

0x080303C8, 428 bytes, THUMB, parked.

Best score so far: not measured.

## What it does

Exchanges this frame's key input with the other players in link play and returns all players' keys ORed together. When link play is off it returns the local keys; a bad word from another player makes it fall back to the local keys or return 0.

## How close it is

Compiles 8 bytes too short (420 of 428). 18.0% of bytes are identical, which means little because of the shift. Control flow, types, constants and both loops are settled.

## What is left

Two small code patterns differ. At two places the original sets the volatile field unk210 to all ones with a single read before the write, where ours reads it twice; and in the outgoing key word the original groups 0x8000 with the shifted field, where the compiler moves it. The first needs a new way to write a volatile read-modify-write; the second has a known but unnatural fix (holding 0x8000 in a local).

## Already tried

- The key word as one plain expression: the constant still moves and is widened, costing an extra pool word.
- Other groupings of the key word (three `|=` statements, OR before the shift, `+` instead of `|`): the constant still moves, or the wrong instruction appears.
- Holding 0x8000 in a local (`int hi = 0x8000`): reproduces the grouping exactly but the total stays 420 bytes. Left out of the draft as unlikely original source.
- `x |= c`, `x = x | c`, and going through a bound struct pointer for unk210: all still read twice.
- volatile pointer forms for the two read-modify-writes: 416 or 412 bytes; they lose the wanted instructions.
- A non-volatile overlay plus an asm barrier: gives the first site's instructions (424 bytes) but spoils the registers; at both sites it builds a second 0xFFFF and gets worse.

## Files

- `sub_080303C8.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

Wave 74 semantic baseline: 420/428 (-8), 18.0%, 343 of 420 candidate bytes differ, first difference +0xe.

### What still differs

The key/state scan still needs the ROM's distinct volatile RMW/dead-read behavior at two sites plus its OR-chain allocation.

### Why it is close

All known faithful volatile pointer forms and both site contexts are now measured and documented.

### Already ruled out

- Faithful volatile pointer forms measured 416/412 bytes.
- A first-site overlay/barrier reached 424 but worsened allocation; applying it to the second site regressed.
- High-half OR grouping remained 420 bytes with 342 differences.

### Settled

- The two volatile sites are not interchangeable; preserve their distinct source contexts.

### Why it is parked

Wave 74 W74-C. Needs a new faithful volatile-C model.

### Wave 93

WAVE 93 (W93-C). UNCHANGED at 420/428 (-8), 17.99%, first difference +0xe. Three probes, all size-neutral, and one base rejected.
BASE REJECTED: `best.c` (19.63%, size-exact) diverges EARLIER than the draft -- first difference +0xa against the draft's +0xe. Its percentage is higher only because it is 8 bytes longer while the draft is short. On a short draft the percentage is not comparable across sizes; compare the first-difference offset. Renamed `best.c.wrongc`.
TWO VISIBLE ROM CONSTRUCTS MEASURED, BOTH BYTE-NEUTRAL. The ROM's key word ends `movs r4,#0x80; lsls r4,r4,#8; adds r2,r4,#0` (0x8000 built in a register and COPIED, i.e. held in a local) and then `ldrh r0,[r3,#6]` -- a dead read -- immediately before `strh r1,[r3,#6]`. Both look like missing source. Neither is: `int hi = 0x8000` in the grouping gives 18.46% and STILL -8; a discarded read `gUnknown_0849B01C->unk06;` gives 16.82% and STILL -8; both together 17.29% and STILL -8.
THE DRAFT ALREADY EMITS THE ROM'S DEAD READ. Every member of struct Unk0849B01C except unk08 is volatile (the header says matched siblings sub_0802F23C and sub_08031B30 only compile that way), so the read falls out of the existing assignment and authoring a second one adds nothing. This closes a lead that reads like an obvious 4 bytes: the visible `adds r2,r4,#0` and dead `ldrh` are ALREADY ACCOUNTED FOR and are not where the missing 8 bytes are.
RESIDUAL unchanged: the two `unk210 |= 0xFFFF` sites, and the spellings the park lists are still the only ones measured.

</details>
