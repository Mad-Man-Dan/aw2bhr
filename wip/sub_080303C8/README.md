# sub_080303C8

0x080303C8, 428 bytes, THUMB, parked.

Best score so far: 48.6%.

## What it does

Exchanges this frame's key input with the other players in link play and returns all players' keys ORed together. When link play is off it returns the local keys; a bad word from another player makes it fall back to the local keys or return 0.

## How close it is

Compiles to the right size (428 bytes) with 48.6% of bytes identical. The draft reads gpKeySt once at the top and keeps it for the return, holds the wait loop's 2 in a variable, and groups the key word's OR the original's way. Not yet checked: that nothing changes gpKeySt during the function.

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

### Wave 95

Base: existing draft (420, -8, 18.0%), kept as `sub_080303C8.w95-start.c`; draft unchanged.
- Lever 1: the ROM copies (`adds r3,r2,#0` / `adds r2,r4,#0` before the wait loop) hold the addresses of gUnknown_0849B018 and gUnknown_02023894, each loaded once through the compiler's .rodata address words. Binding those addresses to locals (`pi = &gUnknown_02023894; pa = &gUnknown_0849B018;`, loop reads `*pi`, `(*pa)->unk04`) is byte-identical to the draft: the copy propagates away. Lever did not transfer; mechanism: the ROM's copies are loop-rotation copies the compiler makes, and a source local is just propagated. The first difference (+0xe) is the ROM holding the address in r4 (ours r2), which a source local does not change.
- unk210 read-modify-write: `w = unk210; unk210 = w | 0xFFFF;` byte-identical (combine forwards the read into the use, the dead first read stays: still two reads). `w = unk210 | 0xFFFF; unk210 = w;` loses BOTH reads (412 bytes). With `w` also used later (`acc += w`) identical to the draft. So a single volatile read before the write is not reachable by a local temp; the lever chapters (narrow-global volatile read) were not enough here.
- Not tried: permuter (draft is 8 bytes short in size).

Permuter (added at end of wave 95): two chained 600 s runs from the draft, 18.0 -> 40.9 -> 48.6, SIZE-EXACT (428), first difference +0xa. Draft = current `sub_080303C8.c` (= `.w95-perm3-start.c`). Changes, checked by reading:
1. `new_var = gpKeySt;` read at the top and `return new_var->previous;` at the end. Differs from the start only if gpKeySt is reassigned during the function (callees sub_080301E8 / sub_0802F460); NOT verified, so treat as provisional. This is what supplied the missing 8 bytes (a live saved-register copy).
2. `new_var2 = 2;` holds the constant in the wait loop's `unk04 != 2` test.
3. In the key word the OR is written with the `0x8000 | unk00 << 10` group first, then `~REG_KEYINPUT & 0x3FF`, then `unk02 << 13` (the same value; the ROM groups 0x8000 with the shifted field, which this ordering reproduces without the local for 0x8000).
Lever 1 (copy of a value into a saved register): transferred through the permuter's `new_var = gpKeySt` form; my hand-written address locals were folded away. Remaining: unk210 read-modify-write still reads twice, r4 vs r2 for the &gUnknown_0849B018 address.

</details>
