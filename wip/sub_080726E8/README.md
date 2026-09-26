# sub_080726E8

0x080726E8, 216 bytes, THUMB, parked.

Best score so far: 56.9% (best.c).

## What it does

Copies a w-by-h block of tilemap entries into a 32-wide BG tilemap at (x, y), adding `base` to each entry and clipping to the 32x32 map. When `flip` is set it mirrors the block left to right and toggles each entry's horizontal-flip bit.

## How close it is

Compiles 12 bytes too long (228 against 216), 18.5% of bytes match; the score means little because the extra code starts near the top (a bigger stack frame) and shifts everything after it. Everything outside the flipped loop is right, including the unflipped loop.

## What is left

In the flipped loop the compiler turns both the source and the destination address into pointers that step each iteration (two extra stack slots, and x loses its register), where the original recomputes both addresses every time. The untried idea is to keep one more value live inside that inner loop so the compiler declines to do this, as the matched sub_0806B120 did by turning two stores into bitfield stores.

## Already tried

- Four other spellings of the source index: all compile the same way.
- The destination as one folded index: wrong for both loops; separate row and column terms are kept.
- Reusing the src parameter instead of the local copy p: the local is right, it puts the load where the original has it.
- `int flip` instead of `u8 flip`: u8 is right.
- One loop nest with the flip test inside: the original has two nests.
- Writing `w - (ix + 1)` explicitly: still makes both stepping pointers and also moves x out of its register; worse.
- Keeping extra pointers live across the two halves: no change, or a bigger stack frame.
- Automatic permuter, 300 seconds x 4 threads: no match (the 57% best.c it left is unreadable output, not this draft).

## Files

- `sub_080726E8.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

228 bytes against 216 (+12), 18.5% identical, first difference at +0xa. Re-verified wave 73 (W73-C). THE DRAFT SCORES 18.5%; the 56.9% trymatch prints as 'best so far' is best.c's score and is NOT the draft -- best.c is 142KB of permuter output and is not a readable draft.

### What still differs

ONE FACT: agbcc strength-reduces both pointers in the FLIPPED arm -- `subs r2,#2` walking the source backwards and `adds r3,#2` walking the destination forwards, plus two extra frame slots for their hoisted initial values, which is where the +12 bytes and the spill of `x` out of r7 come from -- where the ROM recomputes BOTH addresses from scratch every iteration and reduces nothing. The ROM's flipped inner loop hoists only `lsls r3,r5,#6` (iy*64) and the 0x400 constant, and its only induction variable is `ix` itself.

### Why it is close

Everything else is byte-exact and must be kept: the UNFLIPPED arm (which the ROM DOES reduce, and which this reproduces instruction for instruction), the prologue, the frame holding map at [sp] and p at [sp,#4], both clip tests, and the register map (ip = w, sl = y, base in sb).

### Already ruled out

- THE ROM'S OWN TWO ARMS ARE THE PROOF THAT SPELLING IS NOT THE VARIABLE: they index the same map with the same clip tests and differ only in whether the compiler reduced. Do not spend attempts on index spellings.
- Source index spelling: `src[iy*0x20 + (w-ix-1)]`, `*(src + (w-ix-1) + iy*0x20)`, `src[iy*0x20 + w - ix - 1]` and `*(p + (w - (ix+1)))` all strength-reduce identically.
- Destination spelling: `map[(y+iy)*0x20 + (x+ix)]` folds the index and scales it once (`lsl #5; add; lsl #1`) and is wrong for BOTH arms; `*(map + (x+ix) + (y+iy)*0x20)` gives the ROM's two separate scalings (`lsls #6` for the row, `lsls #1` for the column). This one WAS worth bytes and is kept.
- `p` as a separate local seeded from `src` versus reassigning the `src` parameter. The local is right: it puts p's ldr/str after the promoted-mode conversion group, exactly where the ROM has it, and the same change closed the siblings sub_080727C0 and sub_0807286C.
- `u8 flip` versus `int flip`. The lone `lsls r0,r0,#0x18` in the conversion group is the u8 parameter's PROMOTE_MODE extension with the `lsr` dropped by combine, so u8 is right.
- Two separate loop nests versus one nest with the flip test inside. The ROM has two nests (the `beq` is above both).
- Wave 57 (W57-D): `w - (ix + 1)` spelled explicitly in place of `w - ix - 1`. Still reduces both pointers AND additionally spills `x` out of r7 into [sp,#0xc], so strictly worse. Note what this means for READING the ROM: its `adds r4,r2,#1` at the top of the flipped inner body, shared between the source index and the loop increment, is a CONSEQUENCE of the giv not being reduced, not a cause of it. Do not try to reproduce that sharing from the source.
- Wave 72 (W72-F): CROSS-ARM POINTER LIVENESS. Keeping the original src parameter live for the unflipped arm is byte-neutral -- the flipped arm still reduces both pointer GIVs and keeps the 16-byte frame. Distinct q/out locals used only by the later arm increase the frame to 20 bytes WITHOUT suppressing either giv.
- decomp-permuter: 300 s, 4 threads, no byte match.

### Settled

- The function blits a w-by-h block of tilemap entries into a 32-wide BG tilemap at (x,y), adding `base` to each entry and clipping to the 32x32 screen block; `flip` mirrors horizontally, which also toggles each entry's HFLIP bit (0x400).
- loop.c discards a giv when `lifetime * threshold * benefit < insn_count`. Nothing reachable from C addresses that comparison directly, and no spelling probed has moved it.

### Notes

Wave 73 (W73-C). The open lever is register PRESSURE, not spelling -- the ROM's flipped arm reads like a loop strength_reduce declined to reduce because too much was already live. Wave 72 ruled out the cross-arm form of that; what remains untried is pressure INSIDE the flipped inner loop itself. Compare sub_0806B120, matched in wave 73, where switching two stores to bitfields added exactly one live constant and that alone stopped strength_reduce from making an element address a pointer giv -- the same mechanism, in the direction this function needs.

</details>
