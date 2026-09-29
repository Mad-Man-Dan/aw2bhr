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
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

Worked across several waves without a match; the full record is the draft's header comment and the notes files in work/sub_0803CFA4/.

### Wave 93

W93-E: not reached; still never permuted.

### Wave 94

W94-B: first permuter run on this function (--current, 900 s, 4 threads, length-penalised scorer). It reported a chain of improvements and KEPT 65.15% -> 73.18% size-exact; the form is WRONG C and is now work/sub_0803CFA4/w94b-perm1-k-dropped.c.wrongc, with two independent errors: a2[0x4CB + k] became a2[0x4CB + 0] in the has-a-unit arm only (so every occupied cell writes its unit byte to the first slot, and the two arms disagree), and the halfword store's offset was bound to an `unsigned char` although k * 2 reaches about 0x4C0 on a full map. Neither is visible to any automatic check: every variable is set before it is read and the length does not change. A second candidate at 73.48% was REFUSED by permute.py's own uninitialised-read guard (reads `cells` before setting it), but trymatch.record_best had already written it into best.c, so best.c/best.json were left recommending a file the permuter had just rejected; best.c is reset to the draft and the refused form kept as best.c.wrongc. The one honest idea in those candidates -- bind the halfword store's address to its own pointer before the store, which is close to what this park's `left` line asks for -- measures 35.39% at +4 bytes on its own (w94b-store-ptr.c), so the whole reported 8-point gain came from the wrong-C parts. Draft restored and re-verified: 660/660, 65.15%, first difference +0xa. The frame residual (32 against the original's 36) is untouched.

### Wave 97

wave 97 (W97-M)
Base: current draft (65.15%, size-exact, frame 0x20 vs ROM 0x24). Un-binding probes (spellings.py, frame meter): dropping the `w` binds -4 bytes (54.7%); dropping the
`q` bind (inline `a2[0x4C4+i]`) byte-identical; deleting `*((u8 *)gPlayers) += 0` 64.55% (still needed, 0 bytes); replacing `rows` by the inline sum at the 3rd read +20;
inlining `e->type` +16. None moves the frame to 0x24; the frame still only ever shrinks when a bind is removed. The brief's "-1 copy, un-bind first" does not match
the measured state: this draft is size-exact and one spill slot SHORT, not one copy over.

</details>
