# sub_0806412C

0x0806412C, 232 bytes, THUMB, parked.

Best score so far: 99.1%.

## What it does

Initialises two tables, gUnknown_0202F140 and gUnknown_0202F110, from ROM data (the first scaled to 20.12 fixed point), then stores its eight u16 arguments in the second table and two globals.

## How close it is

Compiles to the right size (232 bytes) with 99.1% of bytes identical. A constant local `new_var = 2` added to the row base moved it from 97.0%. What is left: a few bytes of register choice around the row address.

## What is left

Find what keeps the first loop's counter counting up with a `<= 7` test while the compiler still builds the 0x18-byte stride pointer itself; every relational test tried so far makes it count down. Separately, get the second loop's base as the table address plus 2 computed in a register.

## Already tried

- Every relational form of the first loop (`i <= 7`, `i < 8`, do/while, the `i = 0` before or after the v7/v8 lines): the compiler rewrites the counter to count down. Only `i != 8` keeps it counting up (kept).
- A goto loop: keeps the counter counting up but loses the compiler-built stride pointer, and writing the stride as a source variable costs 8 bytes.
- Using i after the loop: the compiler substitutes its final value 8; no effect.
- Binding gUnknown_0202F110's base to a local: much worse, an extra read-only constant and indirect loads at every use.
- Writing `a7 * 0x1000` and `a8 * 0x1000` inline at their stores: a bigger stack frame; binding them to locals first is required (kept).
- The permuter's saved best (best.c): shares the table constant as the ROM does, but compiled, its first loop counts down again.

## Files

- `sub_0806412C.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

90.95% -- 232 bytes, SIZE-EXACT (was 56.90% at wave 93 start)

### What still differs

TWO facts. (a) check_dbra_loop REVERSES the first loop's counter here and did not in the ROM. The ROM keeps `movs r1,#0` in the preheader with `adds r1,#1; cmp r1,#7; ble` at the bottom ALONGSIDE the 0x18 giv; the draft emits `movs r3,#7` with `subs r3,#1; cmp r3,#0; bge` and no separate init. Same instruction count. It also costs the second loop's `adds r3,r1,#1` / `adds r1,r3,#0` shuffle, which is that loop reusing r1 for both the counter and the row pointer. (b) The second loop's base comes out as ONE pool word relocated `gUnknown_0202F110+0x2`, where the ROM materialises the bare symbol and biases it (`ldr r0,=g; adds r4,r0,#2`) so that the one pool word is shared with the six halfword stores after the loop -- the ROM has six pool words, the draft seven.

### Why it is close

Everything else is exact: the eight-parameter prologue with four u16s spilled to a 0x10 frame and read back with `ldrh`, both `stm`/`strb` copy loops, the `ldrsh` walk with `* 0x1000`, the six halfword stores and the two 20.12 globals. The size already matches.

### Already ruled out

- LOOP FORM does not move the reversal: `for (i = 0; i <= 7; i++)`, `for (i = 0; i < 8; i++)` and `do { } while (++i <= 7);` all reverse, with the inner loop written ascending or descending, and with `i = 0` placed before or after the v7/v8 statements
- writing the outer loop as a GOTO loop DOES keep the counter ascending -- loop.c never sees it -- but it also loses strength reduction, so the 0x18 stride must be an explicit source variable, and agbcc then hoists that variable's increment ABOVE the inner loop where the ROM has it at the bottom next to the counter. The ROM needs BOTH the giv and the un-reversed counter and nothing tried gives both.
- binding gUnknown_0202F110's base to a `u8 *` local to force the ROM's two-instruction preheader is much WORSE, not better: agbcc emits a `.LC` rodata word holding the address and loads through it indirectly at every use, costing four instructions and a rodata entry
- wave 46 (W46-K) re-confirmed W46-B by probe: hoisting `i = 0` into a statement of its own placed BEFORE the v7/v8 statements, with the loop written `for (; i <= 7; i++)`, still reverses; so does the same init with `do { ... i++; } while (i <= 7);`. The ROM's `movs r1,#0` sitting ahead of the v7/v8 insns is NOT reachable by moving the source init.
- WAVE 73 (W73-G): THE ENTRY'S LAST UNTESTED IDEA IS REFUTED. `A SURVIVING USE OF THE INDEX` does NOT block check_dbra_loop when the use is AFTER the loop: adding `gUnknown_030005F4 = i;` immediately after the first loop still reverses it (`movs r3,#7` / `subs r3,#1` / `cmp r3,#0` / `bge`) and satisfies the added use with a constant, `mov r3,#8` -- gcc knows the biv's final value, so a use after the loop costs the transformation nothing. Measured by compile_probe.
- WAVE 73 (W73-G): THE DISCRIMINATOR IS AN IN-BODY USE THAT SURVIVES STRENGTH REDUCTION, and this function contains its own control: the SECOND loop does not reverse (`mov r3,#0` / `add r3,r3,#1` / `cmp r3,#5` / `ble`) because its address arithmetic is `lsl r0,r3,#3` -- `i * 8` is a single shift, which strength_reduce does not find profitable, so `i` keeps a live in-body use. The first loop's `i * 0x18` IS profitable, is reduced to the accumulating giv, and leaves `i` used only by the exit test, which is exactly check_dbra_loop's precondition. So the ROM's source must contain an in-body use of `i` that is not itself reducible; nothing in the settled statement set supplies one, and that -- not the loop form, not the init position, not the goto -- is the one question left.
- WAVE 73 (W73-D): THE GOTO LEVER IS CONFIRMED NON-TRANSFERABLE, and the reason is now a documented rule rather than an observation. sub_080344F0 was blocked on the IDENTICAL check_dbra_loop reversal and MATCHED this wave on the goto form -- but only because its loop body is a bare byte copy already written with explicit `s++`/`d++` source bivs, so switching loop.c off cost it nothing. This function's outer loop needs the 0x18-stride giv that only strength_reduce produces, so the goto form trades the reversal for the giv and cannot win. See the new chapter 'A GOTO loop is the lever on check_dbra_loop when the body's pointers are ALREADY explicit source bivs' in docs/agbcc-codegen.md -- the test to apply before reaching for the lever is whether the body needs a giv at all. Do not re-probe the goto form here; W46-B's result stands and is now explained.
- WAVE 77 (W77-J): THE REVERSAL IS BLOCKED BY AN INEQUALITY-FREE EXIT TEST. `for (i = 0; i != 8; i++)` does NOT reverse: the counter comes out ascending, `mov r3,#0` in the preheader with `add r3,r3,#1 / cmp r3,#8 / bne` at the bottom, ALONGSIDE the 0x18 giv -- exactly the ROM's shape except for the test form. check_dbra_loop only handles a relational comparison; NE makes it bail. So the three-wave conclusion 'no loop form reaches the reversal' is WRONG -- what is true is that no form with a RELATIONAL exit test reaches it. The open question is now much narrower: what blocks check_dbra_loop while KEEPING the ROM's `cmp #7 / ble`.
- WAVE 77 (W77-J): WITH THE REVERSAL BLOCKED, HOISTING `i = 0;` AHEAD OF THE v7/v8 STATEMENTS PUTS THE INIT EXACTLY WHERE THE ROM HAS IT -- `mov r3,#0` immediately after the two `ldr`s and BEFORE `lsr r6,r6,#4 / mov ip,r6 / lsr r0,r0,#4 / mov r8,r0`. W46-B and W46-K both measured this hoist and both recorded 'the ROM's movs r1,#0 ahead of the v7/v8 insns is NOT reachable by moving the source init'. That was measured in the WRONG CONTEXT: with the counter reversed there is no init insn to place, so the probe answered about a loop that had no init. Wave 16's inverted-width-sweep rule again. The init position IS reachable and is now in the draft.
- WAVE 77 (W77-J): AN EXPLICIT `off` SOURCE VARIABLE IN PLACE OF THE GIV (`off = 0; ... dst = (int *)((u8 *)gUnknown_0202F140 + off); ... off += 0x18;`) costs +8 BYTES, 240 vs 232, 40.9%. So the W46-B goto experiment's second half is refuted independently of the goto: spelling the 0x18 stride as source is not merely 'lost strength reduction', it is eight bytes worse on its own. The 0x18 giv must come from strength_reduce.

### Settled

- BINDING `a7 * 0x1000` AND `a8 * 0x1000` TO LOCALS IS LOAD-BEARING and was the whole first half of the fix. Written inline at their two stores, the address of gUnknown_0202F140 wins ip, a7 spills to a fifth frame slot (`sub sp,#0x14` instead of `#0x10`, every stack argument +4) and the address is LICM-hoisted out of the first loop -- where the ROM rematerialises it every iteration. With v7/v8 bound, ip and r8 hold them, the frame is 0x10 and the hoist is gone. This is the wave-17 'two address constants tie and the loser is rematerialised' rule seen from the winning side.
- All eight parameters are u16 and the first four really are spilled to the frame as whole words and read back with `ldrh` -- that is PROMOTE_MODE plus pressure, not a narrower local.
- gUnknown_0858089C is `const s16[24]` read with `ldrsh` and scaled `* 0x1000`; gUnknown_085808CC is `const u8[24]`. Both are walked by ONE pointer that is never reset between rows, so the counts are totals, not strides.
- gUnknown_0202F140 is eight 0x18-byte entries of two three-word vectors, and gUnknown_0202F110 is six 8-byte rows of `u16 + u8[4]`. Both layouts are corroborated independently by sub_08064214, which is MATCHED.
- wave 46 (W46-K): the counter reversal is the SOLE cause of the first loop's diff -- the giv itself is already correct. The candidate emits `adds rD, r5, r2` with the same operand order as the ROM's `adds r3, r5, r2` and differs only in the destination register, which follows from r3 being consumed by the reversed counter. Fixing the reversal is expected to fix the second loop's `adds r3,r1,#1` / `adds r1,r3,#0` shuffle with it. Defect (b), the `gUnknown_0202F110+0x2` pool word, is worth ONE pool word and is independent of (a). The draft is SIZE-EXACT; its 52.2% is positional and must not be read as a wrong shape.

### Why it is parked

Two loop-optimiser facts, not source semantics. WAVE 77 (W77-J) moved this from 52.2% to 56.9% and split the first defect in two. The counter reversal is now KNOWN to be blockable -- an `i != 8` exit test blocks check_dbra_loop and produces the ROM's ascending counter next to the giv -- and the init position, previously believed unreachable, falls out of hoisting `i = 0;` above the v7/v8 statements once the init exists. What is left of defect (a) is exactly two things: the exit test is `cmp #8 / bne` where the ROM has `cmp #7 / ble`, and the outer loop's counter and row pointer hold the OPPOSITE registers from the ROM (candidate counter r3 / rowptr r1, ROM counter r1 / rowptr r3). The one question is what suppresses check_dbra_loop with a RELATIONAL test still in place; every relational form (`<= 7`, `< 8`, do/while, init hoisted or not) reverses, and NE is the only thing measured that does not. Defect (b), the `gUnknown_0202F110+0x2` pool word, is untouched and independent.

### Wave 93

- **result:** 56.90% -> 90.95%, still size-exact, first difference +0x40 -> +0x45
- **base_adopted:** best.c/recovered.c (85.34%), audited as equivalent C and adopted. Its changes: the first loop rewritten for (i = 0; i <= 7; i++) where the draft had i != 8, the second loop's test written (i + 1) <= (5 + 1) where the draft had i <= 5, and a temporary holding gUnknown_0202F110's base (renamed new_var -> entries). All three preserve values: same iteration counts, an array address is a constant, no read before set, and this function calls nothing.
- **permuter:** 900 s x 4 threads from the 85.34% base: 85.34% -> 88.79%. Three mutations, each audited as equivalent: v8 = a8 * 0x1000 sunk into the inner loop (both loops have constant bounds so it always runs and a8 never changes), j = i used as the row index before the inner for reassigns j, and i = 4 written before the [4] store. Do not tidy these.
- **hand_gain:** 88.79% -> 90.95% by moving i = 0; out of the for-init to its own statement immediately before v7 = a7 * 0x1000;, with the loop written for (; i <= 7; i++). The ROM emits the counter's init one instruction earlier than the candidate did (ROM counter in r1 and row pointer in r3; the candidate had them swapped). Five positions measured: at the top 88.79%, after src 89.66%, after tbl 90.95%, after v7 88.79%, left in the for-init 88.79%. A single optimum, not a direction.
- **negatives_corrected:** TWO recorded ruled-out axes here were combination-specific and are false as stated. (1) i <= 7 was recorded as always reversing the counter; it does not, once the second loop is spelled (i + 1) <= (5 + 1) -- worth 28 points. (2) i = 0 placed before or after the v7/v8 lines was recorded as ruled out; under the new base it is worth 2.2 points. Read every ruled-out line in this entry as scoped to the exact combination it was measured in.
- **residual:** 232/232, 21 of 232 bytes differ, first difference +0x45, outer counter and row pointer transposed.

### Wave 94

W94-A: one run from the 90.95% draft returned no kept improvement.

### Wave 97

wave 97 (W97-G)
Base: drafts.py named best.c (90.95%). Its second loop was written `(i + 1) <= (5 + 1)`. **Moved to 97.0% size-exact
(7 of 232 bytes differ, first difference +0x71)** by respelling that loop as the ROM's own shape: the next index
is computed FIRST and assigned back at the bottom, so the counter and the row pointer can share a register:

    for (i = 0; i <= 5; ) { int k = i + 1; entries = gUnknown_0202F110; base = (u8 *)entries;
                            q = base + 2 + i * 8; ...copy loop...; i = k; }

That removed the `adds r3,r1,#1` reorder and the `cmp r0,#6` shape (90.95% -> 96.1%); binding the base as a `u8 *`
and adding the 2 as its own term (`base + 2 + i * 8`) gave the last point (96.1 -> 97.0).
Reading the file: the `v8 = a8 * 0x1000` sits inside the inner loop (same value each pass, kept from the permuter
base; hoisting it was not re-tested); `i = 4; ...[i].unk00 = a5` is the same 96% file's costume. wrongc.py: OK (400 seeds).
Permuter (900 s, 22,738 it, from the 96.1% file): no improvement.

Residual (7 bytes): the ROM makes `entries + 2` a loop-invariant of its own (`ldr r0,=g; adds r4,r0,#2`, then
`lsls r0,r1,#3; adds r1,r0,r4`); ours adds the 2 after the shift (`adds r0,#2`). Every spelling that makes the +2 a
separate statement (`base += 2`, `base = base + 2`, `(u8 *)entries + 2` bound) is folded by cse into a
`gUnknown_0202F110+0x2` pool word (60.8%, seven pool words) -- two states only: folded pool word, or +2 after the
shift. `2 + base + i * 8` is byte-identical to `base + 2 + i * 8`.

Proposed summary: does = fills the eight 3-word vectors from the ROM table scaled to 20.12, the six 4-byte rows,
then stores the eight arguments. status = 97.0% size-exact. left = `+2` of the row base is added after the index
shift instead of hoisted with the base. tried = loop-shape respelling (moved), base binds (above), permuter.

wave 97 (second pass)
Base: unchanged 97.0% draft. Goal: make `entries + 2` a loop-invariant of its own (`ldr r0,=g; adds r4,r0,#2`).
Measured (spellings.py, 15 variants): `entries[i].unk02` / `gUnknown_0202F110[i].unk02` (array member): 60.8% (folded into a `g+0x2` pool word); `base = (u8 *)&g[0] + 2` and `base = g[0].unk02`: 59.9%; `base = (u8 *)g; base += 2`, `+ (u16)two` block local, entries bound before the first loop: size +8, frame 0x14 (the address goes through a `.LC` rodata word and a7 spills).
ONE spelling produces the ROM's separate `adds r5,r4,#2`: bind `entries = gUnknown_0202F110; base = (u8 *)entries + 2;` between the loops AND write the six trailing stores through `entries[..]`. That gives `add r5,r4,#2` and `q = base + i*8` but the bound `entries` stays live to the stores (r4 held, size -4, 67.7%), where the ROM reloads `ldr r0,=g` after the loop (the pool word is shared). Any spelling that leaves the trailing stores bare (`gUnknown_0202F110[k]`) after the bind switches to the `.LC` indirect word (size +8). Re-binding `entries = gUnknown_0202F110` again before the stores (or a second pointer `e2`, or `entries = 0;` first) also switches to `.LC` (+8).
Conclusion: the ROM needs the bound copy dead after the loop AND bare-global stores that share the same pool word; every spelling gets one of those two, not both. Not matched.
Proposed summary addition (tried): `+2` as a member/array-member address, `(u8 *)` walker with `+= 2`, bind before either loop, bind between loops with the stores through the bind (-4), re-bind before the stores (+8).

wave 97 (W97-PG)
Permuter chain: 2 links, 96.98% -> 99.14%. Kept link1: constant new_var = 2 used in q = (base + new_var) + i*8 (benign constant). Link2 NO-IMPROVEMENT. Start files .w97pg-perm1/2-start.c.

</details>
