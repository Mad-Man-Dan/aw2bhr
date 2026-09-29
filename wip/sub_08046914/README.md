# sub_08046914

0x08046914, 368 bytes, THUMB, parked.

Best score so far: 80.7%.

## What it does

Draws the text of an info panel at x position a for entry b of gUnknown_085D583C (20-byte records with a name id and a defence value, likely terrain types): the name centred on row 1, text 0x960 on row 3 (plus 0x969 when gUnknown_02028DD4 is 0), on row 5 either text 0x969 or the number sub_08026C6C(b) returns, and text 0x961 on row 7 if any of the three unit types listed in gUnknown_084C20C0 has a non-zero repairTable entry for b.

## How close it is

Compiles to the right size (368 bytes) with 80.7% of bytes identical, from the same alias lever as sub_08046030. What is left: the frame is 8 bytes where the original's is 12; the original spills a + 0x38 and reloads it with an unsigned shift.

## What is left

The draft keeps one value too many in registers, so it never spills: the original saves `a + 0x38` to the stack and re-reads the text-buffer pointer through its address before every call, where the draft keeps both the address and the loaded pointer. The draft's own reading is that the original had more locals here; splitting `t` or the 0x8000 and 0 constants into their own locals is the untried next step.

## Already tried

- Writing the centring as one inline expression: the compiler reorders it and loads 0x50 before the call; the separate `x = ...` statement is required.
- `(u32)` casts on the two `/ 8` divides: 8 bytes worse.
- Loading the text-buffer pointer in its own statement or inside the first call's argument: moves the load but rotates the registers of a, b and the name pointer and shrinks the frame to 8 bytes.
- Volatile or cast spellings of that load: byte-identical.

## Files

- `sub_08046914.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 356/368 (-12), 14.9%. The early gUnknown_08499578 value/address lifetime split remains. Binding, comma placement, volatile and cast probes either rotate a/b/gfx and shrink the frame or compile identically; call sequence and body shape are settled.

### Wave 95

Base: the old draft (kept as `sub_08046914.w95-start.c`, 14.95%, size -12, first difference +0xe). Draft left as that file.

Pre-registered step (split the 0x8000 / 0 constants into their own locals): tried. `hi = 0x8000; zero = 0;` used in calls 1-3 only and literals after: size -12 -> -8, 12.5%, first difference +0x12 (kept as `sub_08046914.w95-hi.c`). The ROM does not create the constants at the top; it makes them inside the first call's argument setup (after the `.rodata`-word load) and keeps them in sb/sl for calls 1-3, and the compiler hoists the locals above the divide instead. Constants used through ALL calls: size -20 (worse). `(u32)` casts on the two `(a + 0x50)/8` and `(a + 0x60)/8` divides: -20. A `static inline u16 *TextBuf(void)` helper for the calls after the first `if`: -20, 7.9%. `u16 **pp = &gUnknown_08499578` bound before the else arms and `(*pp)` there: -24.

What the diff shows the ROM doing: calls 1-3 reach the pointer variable through the `.rodata` word (held in r8, reloaded per call), but the else arms and the later calls use a PLAIN `ldr =gUnknown_08499578`. The draft (and every variant) unifies the later uses with the word-derived value. Same unexplained split as sub_08046030's block A; none of the levers tried creates it in either function.

Permuter (two chained runs from the `hi` variant): reports 11.41 -> 68.21 -> 68.75%, size-exact. THESE ARE WRONG C AND MUST NOT BE USED: the kept file (`sub_08046914.w95-perm1.c`) turns `/ 2` into `x = 2; x = f(gfx) / x` which compiles to a `bl __divsi3` call instead of `lsrs; adds; asrs`, drops the `(s16)` cast, adds a `volatile int pad` local (second run) and reuses `zero = t` in the loop. The size-exact score is an accident of the call replacing the shift sequence. Read the diff, not the banner: the first difference is still at +0x12.

Proposed summary:
- does: lays out one unit's info panel: the portrait, two text rows, an optional third, and a per-terrain icon
- status: 14.95% at -12 bytes
- left: the ROM reaches the text-buffer pointer through a `.rodata` word for the first three calls and through a plain literal for the calls after the first `if`; the draft reuses one value for all and so never spills the way the ROM does
- tried: constant locals for the first three calls (-8), for all calls (-20), inline text-buffer helper (-20), `&gUnknown_08499578` bind (-24), unsigned casts on the divides (-20); permuter's 68% result is wrong C (calls __divsi3)

### Wave 97

wave 97
Base: sub_08046914.c (== w95 draft, 14.95%, size -12, first diff +0xe). `best.c` (68.75%) rejected again: it is the `__divsi3` / `volatile pad` permuter output described in wave 95 and drafts.py tags it `[new_var]`; no valid twin found (the `/ 2` stays a signed shift in the draft).
Dumped the draft with `tools/rtldump.py sub_08046914 --flags=-da` (a refinement of the wave 95 finding): in the draft EVERY use of the text-buffer pointer goes through the SAME compiler-made pool word (`mem/u (symbol_ref/u *.LC2)` with REG_EQUAL `symbol_ref gUnknown_08499578`), and cse keeps that form in all of the later else arms too (the loads are re-issued after each branch merge, so the "one value for all" reading in wave 95 is only true for the first three calls). The ROM uses the `.rodata` word (0x0812A110) for the first three calls and a plain `ldr =gUnknown_08499578` for the calls after `sub_08026C6C`. So the missing piece is not a split of one pseudo but which constant the later loads reference: the ROM's later loads are the plain symbol constant. A source that makes cse replace the word load by its REG_EQUAL constant (address value used directly) after the first `if` is the thing to find; not found.
No new probes beyond the dump. Same construct as sub_08046030 (see its wave 97 note: there cse's skip-blocks path carries the first block's pseudos over `if (x <= 0x63) a++;`).

wave 97 (W97-R)
Base: `sub_08046914.w97-start.c` (14.95%, -12). Now 80.71%, SIZE EXACT (368), first difference +0xA (frame: ours `sub sp,#8`, ROM `#0xC`).
The lever transferred from sub_08046030: gUnknown_08499578 and gBG0TilemapBuffer are one address (linker alias, aw2bhr.lds:34) but two symbols to the compiler. Calls 1-3 keep `gUnknown_08499578` (reached through the compiler-made `.rodata` word, held in r8), the two calls in the `else` arm and the loop call use `gBG0TilemapBuffer`, which gets the plain literal the ROM shows. With the hi/zero constants held for calls 1-3 only this is size-exact (13.6%); three chained permuter runs then took it 14.7 -> 67.4 -> 77.7 -> 80.4 -> 80.7%. Every kept change was read: a copy `hiInit = 0x8000; hi = hiInit`, `long t` (not u32), a temp `buf = gUnknown_08499578` used only in call 1, a temp `aCopy = a` used for the `(a + 0x50) / 8` in the first else arm, and `(unsigned short)` on the `(a + 0x60) / 8` argument. All valid; wrongc.py OK (400 seeds). One `(double) 5` and one assignment-in-expression the permuter made were cleaned.
Residual: the ROM spills `a + 0x38` (`str r0,[sp,#8]`, reload with `lsrs`), ours keeps it in a register (`asrs`); `long t` gives the asrs, `u32 t` gives the lsrs but 25.8% (the spill vanishes). Frame is 8 not 12 for that reason.
Proposed summary:
- does: lays out one unit's info panel: the portrait, two text rows, an optional third, and a per-terrain icon
- status: 81% at the right size; frame is 4 bytes small
- left: the ROM spills `a + 0x38` to the stack and reloads it (unsigned shift); the draft keeps it in a register
- tried: alias name for the text buffer in the later calls (the lever), constants held for calls 1-3, copy temps found by the permuter (4 chained runs)

</details>
