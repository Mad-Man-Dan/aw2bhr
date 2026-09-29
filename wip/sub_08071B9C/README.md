# sub_08071B9C

0x08071B9C, 232 bytes, THUMB, parked.

Best score so far: 18.8%, +8 bytes (best.c).

## What it does

One frame of a 16-colour palette fade run by a proc. It blends each colour's red, green and blue from start to target by step / step count (via Interpolate), writes the result and uploads the palette, then advances the step; when the fade is finished it ends the proc instead.

## How close it is

Compiles 8 bytes too long (240 against 232), 19% of bytes match; the score means little because the difference starts near the top and shifts everything after it. The loop shape and the registers of the main pointers are right; the extra bytes are in four colour-mask steps.

## What is left

In four of the six mask steps (red and green of each colour) the original loads the mask into the result register and ANDs the colour into it, while the draft copies the colour and ANDs it with a mask kept in another register, because the compiler shares the 0x1f mask across both colours. Change what is live around the mask code so the compiler stops holding 0x1f in one register across both colours; respelling the constants cannot do it.

## Already tried

- A for or while loop: the compiler turns all three arrays into stepping pointers and counts down; the original counts up with one stepping pointer, which only a goto loop reproduces (kept).
- `*dst++` inside a real for loop: the other two arrays still become stepping pointers.
- Changing the declaration order: fixes the stack-slot order (already right), not the registers.
- `do { } while (0)` around the three Interpolate calls: gives the proc and fade-record pointers the original's registers (15% to 19%) but leaves the mask copies unchanged; kept.
- `do { } while (0)` around the whole goto loop: breaks the loop shape.
- Six separate constant locals, one per mask: byte-identical, the compiler folds them away.

## Files

- `sub_08071B9C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

15.1% identical, candidate 240 bytes (+8), first difference at +0xc. Wave 35, re-measured wave 57 (W57-D) and again by W59-E in wave 59 by exit status -- unchanged.

### What still differs

THE SCORE IS POSITIONAL AND MEANS NOTHING HERE -- the shape is right. The whole 8 bytes is FOUR extra instructions that all trace to a single register-allocation choice: the ROM puts 'proc' in r7 (low) and 'data' in sl (high); the draft puts 'data' in r7 and 'proc' in sl. Two consequences, and together they are the entire diff. (1) Every one of the four proc->unk2C reads (three Interpolate call sites plus the tail) needs a 'mov rN, sl' first; the ROM pays that cost on 'data' instead, and 'data' is read fewer times. (2) The 0x1f / 0x3e0 / 0x7c00 masks change hands: the ROM rematerialises each per use and lets the two-address 'ands' CONSUME the constant ('movs r1,#31; ands r1,r0'), while with r7 holding 'data' the draft has a spare low register, so the masks stay live and are shared between the 'a' group and the 'b' group, forcing a copy at each use ('adds r1,r0,#0; ands r1,r3'). The register swap is the cause; the constant sharing is the symptom.

### Why it is close

LOOP SHAPE IS SETTLED -- do not re-litigate it. Written as a for/while, strength_reduce turns data[i], src[i] AND dst[i] into three walking pointers and check_dbra_loop reverses the counter to 'subs; cmp #0; bge'. The ROM has an ASCENDING counter, a shared 'lsls r2,r1,#1' index and exactly ONE walking pointer, which is only reachable as a goto loop (no loop notes -> no LICM, no giv, no dbra) with '*dst++' as a source-level variable. 'i' and 'dst' living in stack slots ([sp,#4] and [sp,#0xc]) is reload spilling, not a source-level choice, and the draft reproduces both slot offsets exactly.

### Already ruled out

- Wave 35 / wave 57 (W57-D): declaration order. It fixes the stack-slot order 4/8/0xc, which the draft already has right, but not the register choice.
- Wave 35 / wave 57 (W57-D): '*dst++' inside a real 'for' loop. Still reduces the other two arrays to walking pointers, so it breaks the instruction stream.
- Wave 59 (W59-E): NOT ATTEMPTED BY ME. This function was on my brief's 'if you finish early' list and I did not reach it; the 15.1% is the pre-existing draft's, re-measured by exit status rather than inherited. Nothing new was ruled out this wave.

### Left to try

THE TENSION THAT HAS TO BE BROKEN, stated plainly by W57-D because it is the actual obstacle: the goto loop is what makes the instruction stream right, and it is ALSO what makes the allocation wrong. With no NOTE_INSN_LOOP_BEG the in-loop references are not weighted by loop depth, so 'data' (7 refs, shorter live range) outranks 'proc' (7 refs, live to the tail) in allocno_compare and takes r7. A real 'for' loop restores the weighting -- and the ROM's r7/sl split with it -- but reintroduces the giv reduction that breaks the stream. So the next lever is NOT another loop spelling. It is either (a) a spelling that changes the reference COUNTS -- drop 'data' to 6 refs or lift 'proc' to 8, which flips floor_log2(n_refs) in allocno_compare's priority and should hand r7 back to 'proc' without touching the loop form -- or (b) a real 'for' loop with enough extra live values that strength_reduce declines to reduce on register-pressure grounds, which is how the ROM's own body reads. Reading proc->unk2C once more, or 'data' once less, is CHEAP TO PROBE AND HAS NOT BEEN TRIED. By W59-F's exhaustion axis this is the higher-value of the two rows added in wave 59: it ends with a named, cheap, untried idea.

### Wave 87

WAVE 87 (W87-D): 15.1% -> 19.4%, still 240/232 (+8), first difference +0xc -> +0x16, draft REPLACED (old in w87-start.c). W80-C `do { } while (0)` lever CONFIRMED: a wrapper around the three Interpolate calls puts proc in r7 and data in sl (the ROM's assignment) and removes all four `mov rN,sl` reads (baseline re-measured this wave: `mov sl,r0 / ldr r7,[r0,#44]` vs ROM `adds r7,r0,#0 / ldr r0,[r7,#44] / mov sl,r0`). Three separate per-call wrappers = same code. Around the whole goto loop with the label inside BREAKS the settled shape (data becomes a second walking pointer `add sl,sl,#2`, shared `lsls r2,r1,#1` destroyed). PARK'S CAUSAL CLAIM REFUTED: the swap is fixed and the constant sharing is UNCHANGED -- two facts. Remaining +8: the ROM lets the two-address `ands` CONSUME a rematerialised mask (`movs r1,#31; ands r1,r0`) for ar/ag/br/bg; the draft copies the value and ands against a separate constant register; only 0x1f is genuinely CSE-shared (r3 spans both groups), 0x3e0 is rematerialised per group yet still takes its own register; the two 0x7c00 results already match. Six constant locals, one per mask assigned right before use: byte-identical (19.4%) -- they fold away. New bound on W86-A: a constant local cannot pin a register for a constant cse has already merged BY VALUE (no distinct pseudo, no live-range start). Next: change what is LIVE at the mask block so cse cannot keep 0x1f in r3 across the two groups; no source respelling of the constant can do it. The park's row (a) (reference counts for the swap) is moot.

### Wave 97

wave 97
Base: draft (18.75%, 240 B, +8). No match.
- Fully inlined (no a/b/ar.. locals, `data->unk00[i] & 0x1f` at each call): 220 B (-12), 21.55%, frame `sub sp,#16` matches. Locals for a and b only: frame grows to 0x14. Commuted masks (`0x1f & a`), u32/u16 type for b, `(b & 0xff) & 0x1f`, `% 32`: no change or worse.
- Mechanism seen in the ROM: all six masks are made before the calls, and 0x1f is rematerialised for the b group (`movs r2,#31`) as well as at the tail (`movs r3,#31`), so regmove can swap the commutative `ands` onto the constant register. cse shares 0x1f across the a and b groups in the draft, which forces the copies. Something must end the constant's life between the groups; nothing tried does.

</details>
