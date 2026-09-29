# sub_0807B7BC

0x0807B7BC, 156 bytes, THUMB, parked.

Best score so far: 66.7%.

## What it does

Loads a string's glyphs into sprite VRAM and measures its width. For each character it finds the glyph in the table gUnknown_08616194, decompresses its graphics to the next sprite tile slot and adds its width to a total (optionally recording each width). It returns the glyph count and optionally stores the total width.

## How close it is

Compiles to the right size (156 bytes) with 51.3% of bytes identical. The outer loop body is wrapped in do { } while (0), total is an int, and the zero constant is held in its own variable; what is left is register assignment.

## What is left

Reinstate the `do { } while (0)` around the whole outer loop body (148 bytes, two of the three register placements right), then find what makes the compiler spend a third saved register: try moving `str++` to the top of the outer body with that wrapper, and run the permuter from that version, which has never been done.

## Already tried

- Writing the outer loop as while or as if + do/while, to control which loop gets its test duplicated: byte-identical; the compiler decides that itself.
- Authoring `str + 1` as its own variable: worse (140 bytes, 3.8%).
- The inner loop as while instead of for, and swapping declaration or assignment order: byte-identical.
- `do { } while (0)` around the whole outer body: outWidths and the total move into registers and the count goes on the stack, as in the original, but only two extra saved registers are used (148 bytes); the old draft was restored because the score fell.
- `do { } while (0)` around the Decompress call or the final store: no change; around the inner loop: only half the improvement.

## Files

- `sub_0807B7BC.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

9.0% identical, candidate 144 bytes (-12), first difference at +0x2

### What still differs

TWO COUPLED FACTS, both allocation/layout, neither a shape. (1) THE REGISTER BUDGET IS ONE SHORT. The ROM allocates SEVEN call-saved registers -- r4=str, r5=g, r6=str+1, r7=tile, r8=outWidths, sb=total, sl=outTotal -- and spills only `count` into a 4-byte frame (str r2,[sp] / ldr r2,[sp] around the Decompress call). The draft allocates SIX -- r4=g, r5=str, r6=tile, r7=count, r8=str+1, sb=outTotal -- and spills BOTH outWidths and total into an 8-byte frame. Same eight values either way; the ROM parks two of them in high registers and pays `mov r0,r8` / `mov r3,sb` at each read, the draft prefers memory. The 12 bytes are the third high register's push/pop/mov (+4), the duplicated outer guard (+4), and the mov shuffles (+4). (2) LOOP ROTATION, SWAPPED BETWEEN THE TWO LOOPS. The ROM rotates the OUTER loop -- test duplicated inline at entry (ldrb r0,[r4] / cmp r0,#0 / beq _0807B838) and again at the bottom -- and leaves the INNER loop jump-to-test (b _0807B82C into a shared cmp r0,#0 / bne). The draft does exactly the opposite. Each version rotates exactly ONE of the two loops and it is the other one.

### Why it is close

Every statement, both loop bodies, the Decompress argument computation, both inline constants (0x000003FF and 0x06010000), the `outWidths[count + 1]` operand order (count is the FIRST operand of the adds), the u16 truncation of total and the (u16)count return are all correct and in the right order. Nothing about the shape, the types or the vocabulary is in question.

### Already ruled out

- Wave 59 (W59-G): SOURCE-LEVEL LOOP ROTATION IS NOT A LEVER. `while (*str != 0) { ... str++; }` and `if (*str != 0) { do { ... str++; } while (*str != 0); }` compile to BYTE-IDENTICAL output -- same 144 bytes, same 9.0%, same registers, same spill slots, same rotation choice, same 130 differing bytes. The front end folds the source guard away and a later pass re-decides the rotation on its own, so which loop gets its test duplicated cannot be spelled. Written up as its own chapter in docs/agbcc-codegen.md.

### Notes

SETTLED READOUTS, do not re-derive. FOUR parameters, in r0-r3; the draft's fifth `void *a5` is unused, costs nothing and is kept only to agree with the existing declaration. `sub sp,#4` is a SPILL SLOT for count, NOT an outgoing argument slot -- Decompress takes two arguments, and r2 is reloaded from [sp] after the call and used as count, which an argument never would be. `count` is int, truncated only at the return (count++ is a bare `adds r2,#1` with no re-truncation, and the return is lsls #16 / lsrs #16); `total` is genuinely u16, its truncation stored back into the variable every iteration. `str + 1` is computed at the top of the outer body and copied back at the bottom in BOTH versions, so it is compiler-invented and must not be authored as a second source variable. WHY the draft cannot simply be told to use sl: outWidths and total are both used in adds/lsrs forms that only encode LOW registers, so a high register costs a copy at every use and gcc weighs that against a spill. NEXT: the rotation is downstream of the allocation, so there is one fact to move, not two, and it is a source form that makes gcc prefer a third high register over a second spill. decomp-permuter's documented case is order-wrong/slot-wrong allocation, which this is -- but `mcp permute` was broken in wave 59, so that axis is UNTRIED, not ruled out.

### Wave 87

WAVE 87 (W87-F, do{}while(0) transfer test): THE LEVER WORKS HERE -- the only one of five that responded, the one whose mis-allocated values live across a `bl`. Thirteen placements, three outcomes: baseline / around Decompress / around the outTotal tail = outWidths and total both spilled, 144/-12, 9.0%; around the inner for / the match then-block / their composition = outWidths -> r7, count spilled, 148/-8; around the WHOLE OUTER while body (r3, and every composition on it) = outWidths -> r7, total -> r8, count AND str+1 spilled, 148/-8, 5.8% -- two of the ROM's three placements bought back, and `count` (the ROM's one spilled value) is now the spill. One try_match on r3 regressed by score (5.8% vs 9.0%, positional) and the draft was RESTORED to the wave-59 baseline (w87-start.c identical); the r3 form is the better starting point and should be reinstated by the next agent. BOUND: the wrapper re-ranks which values win the registers already being allocated; it does NOT change how many callee-saved registers the function spends (r3 still pushes two hi registers with an 8-byte frame vs the ROM's three and 4 bytes) -- that number is global_alloc's cost weighing, and outWidths/total are used in adds/lsrs forms that only encode LOW registers, so a hi register costs a mov per use. Next, sharper than the park's: with the r3 wrapper only `str + 1` remains in the spill set that the ROM keeps in r6, and str+1 is compiler-invented (must not be authored) -- so probe `str++` moved to the TOP of the outer body (before the inner for) with the compare re-read, composed with r3; and a permuter run from the r3 draft (never run; the wave-59 blocker is gone). Do not re-run r1/r5 (neutral) or r2/r4/r6/s2/s4 (worse than r3).

### Wave 95

Base: the `do { } while (0)` around the whole outer body (148, -8, 5.8%), kept as `sub_0807B7BC.w95-perm1-start.c`; original draft kept as `sub_0807B7BC.w95-start.c` (144, 9.0%). `str++` at the top with the compare re-read (136), `nx = str + 1` temp (144, 10.3%), bare copies of outWidths / outTotal (148): no gain; the copies propagate away.
RESULT: SIZE-EXACT (156), 51.3%, first difference +0xa. Draft = `sub_0807B7BC.w95-perm3-start.c` = current `sub_0807B7BC.c`. Permuter chain (600 s each): 5.8 -> 46.8 -> 51.3 (both semantically identical to the start, checked by reading). What it changed: `total` is `int` (the u16 store at the end truncates identically), a variable `new_var = 0` stands for the zero in the two `!= 0` loop tests, `new_var2 = 0` stands for the NULL test on outTotal, and the return sits inside the `do { } while (0)`. Runs 3 and 4 (58.3, 59.0) were WRONG C: `new_var = tile;` is written inside the glyph loop over the very variable used as the zero constant. Kept as `.w95-WRONG-58.c` / `.w95-WRONG-59.c`. Rewriting that step with a distinct temp (`tc = tile`) is 51.3%, the same as before it, so the improvement was the clobber.
The park's "one register short" is gone: the wrapper plus zero-variable form saves the third high register; what is left is register choice (str in r5 not r4, etc.) inside a size-exact body.
Lever 1: does not transfer as a source form (copies fold away); the permuter's zero-variable is the working equivalent. Comments in the draft are the permuter's, not to be promoted as is.

### Wave 97

wave 97
best.c is WRONG C (wrongc: `new_var = tile` inside the glyph loop, the same clobber as the wave-95 WRONG files); not used.
Base: the size-exact 51.28% draft (w97-start). Rewriting from scratch with the wave-96/97 levers:
* `u16 total` as the ROM has it (per-iteration `lsls/lsrs` into a hi register) and a copy-back step
  (`nx = str + 1` at the top of the outer body, `str = nx` at the bottom) put in front of the wrapper draft: 25%, frame 8.
* Removing the permuter's zero variable only in the inner `for` test (`g->unk00 != 0`) while keeping it in the outer `while`
  and for the outTotal test, plus u16 total and the nx copy-back: **size-exact 156, 57.05%** (was 51.28); kept as e3 below if the
  draft file is that version. Removing the zero variable from all three tests: 152/144 and worse.
* `total = count = 0;` / `count = total = 0;`: byte-identical.
Residual on the 57% form: the ROM keeps `str+1` in r6 and spills only `count` ([sp] = count); we keep `count` in r7, `tile`
in r6 and spill `nx` in r3 across the Decompress call (frame 8 vs 4), and the zero variable still occupies `sl`.

Update (end of wave 97): permuter from the 57.05% form: 57.05 -> 66.03 -> 66.67, size-exact 156, first diff +0xc. Both runs are
valid C (read): `tile & 0x3ff` / `<< 1` / `<< 4` split into temporaries, `0x06010000` and `4` and `8` held in ints, `*str` read
into a u8 before the compare, `nx = str + 1` inside the do-block. Names are still `new_var*`; the residual is unchanged in kind
(callee-saved assignment: the ROM keeps `str+1` in a low register and spills only `count`). Draft file is the 66.67% form.

</details>
