# sub_080726E8

0x080726E8, 216 bytes, THUMB, parked.

Best score so far: 56.9%.

## What it does

Copies a w-by-h block of tilemap entries into a 32-wide BG tilemap at (x, y), adding `base` to each entry and clipping to the 32x32 map. When `flip` is set it mirrors the block left to right and toggles each entry's horizontal-flip bit.

## How close it is

Now compiles to the right size (216 bytes) where it used to be 12 too long, with 56.9% of bytes in place. The flipped loop no longer builds stepping pointers the original does not have. What is left is one stack slot: this build keeps three local values on the stack where the original keeps two, and that shifts the registers everywhere.

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
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

56.94% -- 216 bytes, SIZE-EXACT (was 17.54% at +12 at wave 93 start)

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

### Wave 93

- **result:** 17.54% at +12 bytes -> 56.94% SIZE-EXACT, first difference stays +0xa
- **base_adopted:** best.c/recovered.c (56.94%), audited as equivalent C and adopted. Its only change is a volatile int temporary in the flipped arm's store holding ix (renamed new_var -> col); the source index still uses ix, so the same cell gets the same value, and volatile on a local nothing else touches adds no observable behaviour.
- **mechanism:** The volatile read forces the value to memory and back, which defeats exactly the strength reduction this park was about -- the first thing that has ever moved this function. It is the right MECHANISM and the wrong CONSTRUCT, and the frame proves it: ROM sub sp, #8 (map at [sp], p at [sp,#4]) against the candidate's sub sp, #12. A volatile local is an addressable object so it costs a third slot, and the ROM has no third stack object at all. The size still comes out exact because the one new slot replaces the two the hoisted giv inits needed.
- **six_spellings_measured:** plain int col temp 17.54% at +12 and BYTE-IDENTICAL to writing no temp; u16 *d bound in its own statement 17.54% at +12 and also byte-identical; 8-bit fold-proof mask on the destination index 18.64% at +4 with NO local; the same mask on both indices 19.64% at +8; u16 ix 10.19% at the EXACT size but with a 4-byte frame because it drops p off the stack; u16 ix and u16 iy 11.82% at +4.
- **what_this_rules_out:** An ordinary local is not a splitter here -- agbcc coalesces it away and reduces exactly as before. Only volatile defeats the reduction, and it cannot do so without adding a frame object. Size-exact is reachable two ways and neither frame is right: the volatile form is one slot long and u16 ix is one slot short. The fold-proof mask is the only partial splitter that needs no local (+12 to +4 on the destination index alone, and applied to the source index as well it costs 4 bytes back), and it is where the next attempt should start.
- **permuter:** 900 s x 4 threads from the 56.94% base: no candidate matched or improved, draft restored unchanged.
- **generalised:** docs/agbcc-codegen.md now carries a chapter on reading the frame-slot count as a spelling lever, measured here and on sub_0801FAC4 in opposite directions.
- **residual:** 216/216, 123 of 216 bytes differ, first difference +0xa. One extra frame slot (12 against 8) and the register renumbering that follows from it, which is also why the previously byte-exact unflipped arm now differs.

</details>
