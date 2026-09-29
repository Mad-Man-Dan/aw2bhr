# sub_0801C090 — parked, register allocation only



Wave 50, W50-K. Every instruction in the draft is the right instruction; the

residual is which register each value lives in. Do NOT rewrite the shape.



## What is already settled (do not re-derive)



- The function mirrors a sprite horizontally when `a1 & 0x1000` (the OAM h-flip

  bit) is set. Mirrored x is `a1 - x - width`, with `x` the 9-bit signed field

  of attr1 and `width` from `gUnknown_0848B56C`.

- `gUnknown_0848B56C` is the 4x4 OAM `{width, height}` table in PIXELS, now

  declared in `include/unknown-globals.h` with the ROM contents that prove the

  layout. `gUnknown_03002B20` is added to the Y (attr0) and

  `gUnknown_030030D0` to the X (attr1) — note that this is the OPPOSITE of what

  the existing header comments on those two globals claim ("the y half of the

  scroll origin pair whose x half is gUnknown_03002B20"). One of the two is

  wrong; this function is direct evidence and nothing was changed.

- `gUnknown_03002F2C` must be re-read every iteration: the `strh` through `dst`

  may alias it. No `volatile` is involved.

- `dw` and `dx` are `s16` locals. Their def is `lsl #16; lsr #16` (agbcc's

  PROMOTE_MODE zero-extends sub-word locals regardless of signedness) and every

  read is `lsl #16; asr #16`. `dx`'s pair is folded by combine into

  `lsl #16; rsb; ... asr #16` because its def and use are in the same block;

  `dw`'s is not, because its def is before the `if (src[1] & 0x100)` merge.



## The finding that unblocked this — worth reusing



`(u16)x = <expr with a 32-bit mask>` is NOT what the ROM compiles. Written

directly, combine merges the store's `zero_extend(subreg:HI ...)` into the

`and`/`ior` and narrows the constant: `& 0xffffff00` becomes

`& 0xff00` (emitted `mov #0xff; lsl #8`), and the truncation then disappears

because the value provably fits. The ROM keeps the pool word `0xFFFFFF00` AND

the `lsl #16; lsr #16`.



Routing the value through a 32-bit local first — `t = <expr>; attr0 = t;` —

blocks that combination and reproduces the ROM exactly. Four spellings that do

NOT work (all narrow): `u16 a = expr`, `a |= C`, `a = (u16)(expr)`,

`u32 a = (u16)(expr)`. Two that do: `u32 t = expr; a = t;` and

`u32 hi = <masked part>; a = hi | <low part>;`. Recorded in

`docs/agbcc-codegen.md`.



## The exact remaining diff



Instruction-for-instruction the draft matches; these registers do not:



| value | ROM | draft |

|---|---|---|

| a1 | `ip` | r7 |

| a2 | `sb` | r8 |

| count | **stack slot `[sp,#4]`** | r9 |

| dw | r8 | `ip` |

| src | r5 | r4 |

| dst | r6 | r5 |

| frame | `sub sp, #8` | `sub sp, #4` |



Everything else follows from `count` being spilled in the ROM and not in the

draft:



- ROM's decrement is `ldr [sp,#4]; lsl #16; ldr =0xFFFF0000; add; lsr #16;

  str [sp,#4]; cmp r0,#0` — the test reads the PRE-`lsr` value. The draft's

  in-register form is `sub #1; lsl #16; lsr #16; cmp`. Same semantics, 1 byte

  of frame and a different instruction each.

- ROM keeps `src[0]` live in r2 across the table-index computation

  (`ldrh r2,[r5]; lsrs r0,r2,#0xe` — the shift goes to a different register),

  so the attr0 statement reuses it. The draft destroys it

  (`ldrh r0,[r4]; lsr r0,r0,#0xe`) and reloads. Same for `src[1]`.

- In the table index the draft's `lsl #2` on the size term is scheduled after

  the shape term instead of immediately after its own `lsr`.



## Wave 60 (W60-A) — measurements, and the permuter IS now attempted



**The draft is `-16` bytes, not `-12`, and it scores 14.2%.** Measured with

`try_match` on the draft itself this wave. Two briefs have quoted `-12`; the

number to work against is 16.



**That 4-byte gap is NOT silent drift, and it is worth saying so explicitly**

because wave 60 (W60-D) found a real case of exactly that signature elsewhere: a

draft losing 4 bytes with nobody editing it, because a *correct* header change

made for another function killed a conversion in it (`sub_080152C0`'s return

type going to `s8` made a `(u16)` cast feeding only a `strh` provably dead). The

mechanism does not need a callee — a global's declared type reaches a function

that calls nothing — so it was a live hypothesis here. It is excluded:



- `sub_0801C090` calls nothing at all.

- It touches four symbols, and `git log -S` over `include/` shows the last

  commit changing any of them is **wave 50** (`gUnknown_0848B56C`, the wave that

  wrote this draft and these notes). `gUnknown_03002F2C` last moved in wave 40;

  `gUnknown_03002B20` and `gUnknown_030030D0` in wave 7.

- It declares no struct or typedef of its own and uses none.



So nothing this function reads has been retyped since the draft was written, and

the `-12` is simply a figure that was wrong in the brief. **Do not go looking for

a lost 4 bytes.**



**`best.c` is a much better starting point than this draft and always was.**

It now holds a **42.5%** spelling. Do not open the draft first next time —

`work/*/best.json` is the index the wave-20 chapter says to sweep, and here it

was worth 28 percentage points over the file everyone reads.



**The permuter was run for the first time this wave (300 s, 6 threads) and it

made real progress: 14.2% -> 42.5%.** It did not close the function. What it

found is worth reading, because all three of its edits are the block/binding

levers rather than value rewrites:



- a `do { ... } while (0)` wrapped around the **entire while-loop**;

- `t = 0x100;` bound to the `t` scratch before `if (src[1] & t)`;

- `new_var = a1 | src[1]` bound inside the else-arm's `& 0xfffffe00`.



That is the same family of lever that closed `sub_0801C4D4` and `sub_0801C640`

in this wave (see the wave-60 subsection of the zero-trip-loop chapter in

`docs/agbcc-codegen.md`). **Chain another run from `best.c`, not from this

draft** — wave 59 and this wave both show the escalation only works from the

previous best.



**Run 2, chained from that `best.c`, went 42.5% -> 44.72% and found no zero

score.** It kept all three of run 1's edits and added two more: an identity

`inline_fn(src)` around the `src[1]` read in the table index, and a `(char)`

cast on that index's `* 4` term. Treat the `(char)` as suspect — it is a

semantic change the permuter cannot know is wrong, and it is the kind of edit to

drop before promoting anything built on this line. Compare the same chain on `sub_0801C2DC` in this wave, which went

81.6 -> 97.1 -> 99.6 -> match on three runs: **there the escalation was large at

every step, here it is a couple of points.** That difference is the signal worth

acting on.



**Run 3, chained again, went 44.72% -> 45.83%. No zero score in any of the three

runs.** So the sequence is 14.2 -> 42.5 -> 44.72 -> 45.83: one large jump when

the block levers first go in, then about a point per run. **That shape means the

residual is not reachable by randomisation.** Do not spend a fourth run; spend

the budget on the spill hypothesis below. For contrast, the chain that works

looks like `sub_0801C2DC` in the same wave — 81.6 -> 97.1 -> 99.6 -> match, large

at every step.



Two practical notes on running it here:



- **The check phase is O(candidates) and this function produces a lot of them:**

  678 output directories, each re-scored twice through `trymatch` after the

  search ends. The search itself finished in its 300 s; the tail took over 20

  minutes and was still going. If the log says `Exiting.` and no

  `found new best score! (0 ...)` line ever appeared, **no candidate can be a

  byte match** and the tail is only updating `best.c` — it is safe to stop.

- **Killing `permute.py` leaves raw `cpp -P` output in `work/<fn>/<fn>.c`.**

  Confirmed here: the draft came back as 6,956 lines of expanded headers and had

  to be restored from a copy. The brief documents this for abnormal exits; it

  happens on a deliberate kill too. Copy the draft somewhere safe first.



## What to try next



Wave 65 tested the concrete spill hypothesis by binding `src[1]` and `src[0]`

to per-arm `u16` locals and reusing them through each attr calculation. It moved

the active draft from 344 to 348 bytes (-16 to -12), but scored only 22.22%,

did not spill `count`, and remained well below the 45.83% permuter best. This

rules out the simple "keep the two source halfwords live" lever; the clean

active draft was restored after the verdict.



1. ~~This is exactly the permuter's case (order-wrong / slot-wrong register

   allocation, instruction stream already correct) — `docs/agbcc-codegen.md`

   says run it for 300 s before a third hand rewrite. Not attempted here.~~

   **Done in wave 60 — see above. It is worth chaining further, not repeating

   from scratch.**

2. Failing that, the lever is making `count` the lowest-priority allocno so it

   spills. Its priority is refs/live-length; the draft's is identical to the

   ROM's by inspection, so the difference is more likely one extra live value

   somewhere in the arms than anything about `count` itself.

3. Do NOT try to fix the mask narrowing again — it is solved. Do not remove the

   `u32 t` intermediates.





# Wave 93 (W93-C) -- the stack slot is reachable, and here is the lever



Start of wave: draft 14.17%, 344/360 (-16), first difference at +0xa.

End of wave: 33.33%, 352/360 (-8), first difference at +0xe.

The draft is now the `w4_mirror_dowhile.c` spelling.



## +0xa IS the frame instruction, and that is the whole park



`push {r4,r5,r6,r7,lr}` (2) + `mov r7,sl` (2) + `mov r6,sb` (2) +

`mov r5,r8` (2) + `push {r5,r6,r7}` (2) puts `sub sp, #8` at exactly +0xa.

Every draft that reads `first+0xa` is failing on the frame size and nothing

else; every draft that reads `first+0xe` has the ROM's two stack slots.

That single number tells you which side of the park you are on. Use it

instead of the percentage, which moves for unrelated reasons.



## The slot is a SPILL, not a volatile local



The ROM writes and reads the counter as a WORD:



    ldrh r0, [r5]        count = *src++

    str  r0, [sp, #4]    <- str, not strh

    ...

    ldr  r3, [sp, #4]    <- ldr, not ldrh

    lsls r0, r3, #0x10

    adds r0, r0, 0xFFFF0000

    lsrs r1, r0, #0x10   <- unsigned, so u16 count is right (confirms W86)

    str  r1, [sp, #4]



A `volatile u16` local would emit `strh`/`ldrh` on a 2-byte slot. This is a

4-byte slot holding a zero-extended u16 in a SImode pseudo, which is what the

register allocator produces when it spills. The wave-93 rule "a volatile

local always creates a stack slot" is true but does NOT apply here: it would

create the wrong slot. Do not spend a probe on `volatile` for this counter.



## What actually creates the spill



Measured, each against the same draft:



    draft                                             -16   +0xa

    do { ... } while (0) around the loop, alone       -16   +0xa

    short count, alone                                 -8   +0xa

    t = 0x100 and hi = a1 | src[1] bindings, alone    -16   +0xa

    src[0]/src[1] in locals, MIRROR ARM only          -12   +0xa

    for (;;) { if (count == 0) break; ... }           -12   +0xa

    if (count != 0) do { ... } while (count != 0);    -12   +0xa

    mirror-arm locals + do { } while (0)               -8   +0xe   <- spill

    do { } while (0) + the two bindings               -12   +0xe   <- spill



Two facts fall out of that table.



1. EVERY NATURAL ROTATION OF THE LOOP IS BYTE-IDENTICAL. `while`,

   `for(;;)` + `break`, and a guarded `do/while` all produce the same

   38.61% / -12 / +0xa output. gcc normalises them long before allocation,

   so no amount of rewriting the loop's own shape will ever reach the frame.

2. THE SPILL NEEDS AN EXTRA LOOP NEST **PLUS** EXTRA PRESSURE, AND NEITHER

   ALONE IS ENOUGH. A zero-trip outer `do { } while (0)` on its own does

   nothing. Extra live values on their own do nothing. Together they move the

   first difference from +0xa to +0xe. The outer loop is what pushes the

   counter out of `local_alloc` (its live range now crosses an outer loop

   boundary); the extra pressure is what makes it lose once it is there.



## Where the remaining 8 bytes are



The ROM holds `src[1]` in r4 and `src[0]` in r2 across the mirror arm and

reloads them after the join; the draft gets part of this. The draft is 352

against 360. This is now an ordinary allocation residual with the frame

right, which is the permuter's case. For five waves it was chasing a

structural difference the permuter could not reach; that is no longer true.



## Bases rejected this wave



`recovered.c` (43.33%, size-exact) reaches its size with a `(char)` cast on a

value that is already 0..12, and a NON-static `inline` identity function that

agbcc emits out of line. Both are value-preserving, so it is not wrong C, but

its first difference is +0xe, the same as the honest spelling, so the extra 8

bytes are padding rather than progress. `best.c` (45.83%, -8) is the same

family.



## W86'S `u16 count` WAS WRONG, AND THE TWIN'S `s16` WAS RIGHT (W93-C)

Final state this wave: **48.89%, 360/360 SIZE-EXACT**, first difference +0xe.

Two chained permuter runs from the `w4` base (33.33%, -8) produced exactly one
semantic change: `u16 count` became `short`. Nothing else in the body moved.
That one declaration is worth 8 bytes and 15 points, and it reproduces the
ROM's loop bottom instruction for instruction:

    ldr  r3, [sp, #4]
    lsls r0, r3, #0x10
    ldr  r5, =0xFFFF0000      <- the ROM's constant, which u16 never emits
    adds r0, r0, r5
    lsrs r1, r0, #0x10
    str  r1, [sp, #4]
    cmp  r0, #0               <- the test reads the SHIFTED value

With `u16 count` the same draft compiles the decrement as
`sub r0,r0,#1; lsl #16; lsr #16` and compares the truncated value -- a
different shape and 8 bytes short.

### The W86 reasoning was backwards

W86 recorded: "`s16 n` for the counter: REFUTED FROM THE ROM without a probe.
The ROM's decrement ends `lsrs r1,r0,#0x10`, not `asrs`, so the counter is
UNSIGNED. `u16 count` is right; do not copy the twin's `s16`." It then drew a
method lesson from it: "the ROM's own shift settles signedness in ten seconds".

**Measured: a `short` counter also ends in `lsrs` here.** The trailing shift is
the 16-bit TRUNCATION of the result, not a sign extension, and agbcc emits an
unsigned one for both declarations because the only use is `!= 0`. So the
`lsrs` never discriminated the two types, and the thing that actually does --
the `0xFFFF0000` add, which only the signed form produces -- was sitting in the
same seven instructions the whole time.

The twin `sub_0801BD00` (src/decomp/c_0801BD00.c) declares its counter `s16`.
It was right, and transplanting it wholesale would have worked. This is the
brief's own warning landing on the wave that wrote it: a ROM reading is a
hypothesis, and refusing to spend one probe on it cost this function six waves.

### What remains

48.89% at the exact size, first difference +0xe, which is the first
instruction after the (now correct) frame. Pure register allocation from here.

## wave 97

Base: `sub_0801C090.c` (79.44%, size-exact; `recovered.c` is 43% and was not used). Final: **90.56%**, size-exact, first difference +0x10 (unchanged offset; the differing bytes below it shrank from 74 to 34).

Hand steps (each measured with a one-unit harness, `build/probe/w97k.py`):
1. The mirrored arm's `remaining |= 0xffffff00 & 0xffff;` compiled to `movs #255; lsls #8` (the constant folded to 0xff00). The ROM loads `0xFFFFFF00` from the pool, ORs, and truncates. Spelling it `remaining |= 0xffffff00; remaining = (u16) remaining;` reproduces the ROM's `ldr; orrs; lsls #16; lsrs #16` and the pool word.
2. The ROM negates the width BEFORE the attr0 computation and shifts back down inside the attr1 expression (`lsls; negs` early, `asrs` late). Written as `neg = -(remaining << 16);` ahead of attr0 and `sum = (neg >> 16) + x; sum += (s16) negWidth;`. The split `sum +=` is load-bearing: written as one expression, combine sees that `& 0x1ff` discards the sign extension of `(s16) negWidth` and drops the `lsls/asrs` pair the ROM keeps (size 356 instead of 360). `hi = (x | attr1) & ~0x1ff` as its own statement puts the ROM's order (mask part before the sum).
3. Permuter run 1 (80.8 -> 86.4): `hi` declared u16 instead of u32. Run 2 (86.4 -> 89.7): `(y | sourceAttr0) & ~0xff` split into its own int; the run also added an int zero compared in the loop test, which I removed (measured byte-identical without it). Run 3 (89.7 -> 90.56): `remaining = sum;` before the final mask, reusing `remaining` as the scratch.

Negative: reading attr1 into a separate `srcAttr1` (the ROM does end with the merged attr1 in r2, not r4) made the size 364 and dropped equal halfwords from 140 to 29, so it is not the lever at this point.

Residual: register numbering in the mirrored arm (merged attr1 in r4, ROM r2; the 0x1ff mask in sl vs r7) and the prologue order of `str r3,[sp]` / `adds r5,r2,#0`. The parked entry's "two copies too many" was not settled: the copy delta is unchanged by anything above.

Proposed summary: does = copies a counted sprite template into the OAM shadow with optional horizontal mirroring; status = "360 bytes, size exact, 90.6%; register numbering in the mirrored arm differs"; left = "the merged attribute word sits in r4 where the original has r2, and the original stores tileOffset after copying the template pointer"; tried = the steps above, the srcAttr1 split.

## wave 97 (second pass)

Base: 90.56% draft (`sub_0801C090.w97-second-start.c`). Now **91.67%, 360 B size-exact** (`sub_0801C090.c`; 30 bytes differ, first +0x10).
Lever: give the mirrored arm's loaded attr1 its own variable typed `int` (`int sa1 = src->attr1;` used for the shift, the 0x1ff mask, the 0x100 test and `hi = (x | sa1) & ~0x1ff`), so the merged `attr1` is a separate pseudo and ends in r2 like the ROM. `u16 sa1` costs +4 bytes (364, 20%), `u32 sa1` -4 (356); `int` is size-exact but shifts arithmetically (`asrs`), so the shift is spelled `(u32) sa1 >> 14` (91.39 -> 91.67).
Residual: the pool word of gUnknown_0848B56C (ROM r3, ours r0), the 0x1ff mask (ROM `ldr r7; mov sl,r7; mov r3,sl; ands r3,r4`; ours `ldr r3; mov sl,r3; adds r1,r4,#0; ands r1,r3`), prologue `str r3,[sp]` order, and the loop-bottom count add/compare pair (r0/r1 swapped). All reload/scratch numbering.
Permuter (900 s, 2 threads) from the 91.67% file: 91.67 -> 92.22. Two edits, both valid C (wrongc.py OK, 117 seeds): the `>> 14` cast spelled `(((u32) sa1) >> 14)`, and the count update goes through a `long long nextCount` temp (`nextCount = count * 0x10000 + 0xffff0000; remaining = nextCount;`). Adopted.


## wave 97 (W97-AB)

Base: 92.22% draft (`sub_0801C090.w97ab-start.c`, size-exact, first diff +0x10). Now **93.61%, 360 B size-exact, first diff +0x41**.
Lever 1 (by hand, 92.22 -> 93.33): walk the `template` PARAMETER itself instead of a fresh `u16 *src = template;` (`count = *(u16 *) template; template = (u16 *) template + 1; ... template = (u16 *) template + 3;`, `((struct SpriteTemplateEntry *) template)->` at every use). This is the wave-86 twin idea, which was allocation-neutral on the old base; on the current base it puts `adds r5,r2,#0` before `str r3,[sp]` like the ROM (the parameter pseudo itself becomes the walker, so its entry copy is emitted with the other parameter copies). The prologue difference is gone and the first difference moved +0x10 -> +0x41. Lesson: re-test a twin transplant after each big move; a negative on one base is not a negative on the next.
Lever 2 (permuter, 560 s, 93.33 -> 93.61): `new_var = ~0xff;` bound at the top and used in the non-mirrored arm's attr0 mask. Valid C (wrongc OK, literal WARN only).
Negatives: `sa1 & 0x1ff` instead of `0x1ff & sa1`, the commuted `0x1ff & remaining` at the second use: byte-identical; table pointer first in the width sum: +4 bytes, 28.9%. `levers.py --chain 3` found no improving lever on the 92.22 base.
Residual (all scratch numbering in the mirrored arm): the width table's pool word loads into r0 (ROM r3, i.e. the ROM's load is live across the r0 chain, so RTL order of the load differs); the 0x1ff mask is copied through r3 / sl (`ldr r3; mov sl,r3; adds r1,r4,#0; ands r1,r3`) where the ROM has `ldr r7; mov sl,r7; mov r3,sl; ands r3,r4`; the hi-mask temp is in r4 not r1; the loop-bottom count is in r2 not r1.
Note: the draft still carries a `long long nextCount` temp for the count update (adopted earlier this wave; wrongc OK, frame unchanged). A plain-C spelling that matches would be preferable.
Proposed summary status: size-exact, 93.6% identical; left: the mirrored arm's scratch registers (width-table pool word r0 vs r3, the mask copy path, one temp).
