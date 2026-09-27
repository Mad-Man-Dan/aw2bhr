# sub_0803CFA4

0x0803CFA4, 660 bytes, THUMB, parked.

Best score so far: 65.2%.

## What it does

Fills a buffer describing the current map: its name, width and height, each cell's tile and unit, and counts of certain terrain kinds (6, 8, 10, 11, 14 and 20) in total and per owner. One caller passes the buffer on with length 0x724.

## How close it is

Compiles to the right size (660 bytes); 230 of 660 bytes differ (65.2% line up). The first difference is the stack frame: the original reserves 36 bytes, the draft 32, and the rest follows from that.

## What is left

The original keeps the per-cell counter k in a low callee-saved register, runs out of spare low registers in the loop body, and so saves the store address to one extra stack slot; the draft keeps k in a high scratch register. Find what makes the compiler place k that way, measuring the frame size (32 vs 36) rather than the score; and keep the line `*((u8 *)gPlayers) += 0;`, which compiles to nothing but is needed.

## Already tried

- Binding the store destination as a typed pointer: gives the original's address instructions at that spot but costs 20 bytes elsewhere.
- Writing the cell reads inline with no intermediate locals: the frame shrinks to 28, the wrong way (4 bytes short).
- An explicit `y * 2` local in the outer loop: it lands where the original computes it, but k moves to another high register and the frame stays 32.
- Splitting the destination address into two statements before the reads: right order at that spot, frame still 32.
- That address split together with an extra `a2 + 0x4C4` pointer bind before the first zero loop (copying a bind that helps in the last loop): 8 bytes short; the original binds that pointer only in the last loop.
- Binding one shared row address for both cell reads: byte-identical.
- A struct member at offset 2 for the halfword store: the compiler still folds the offset into the store instruction.

## Files

- `sub_0803CFA4.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

Worked across several waves without a match; the full record is the draft's header comment and the notes files in work/sub_0803CFA4/.

### Wave 93

W93-E: not reached; still never permuted.

</details>
