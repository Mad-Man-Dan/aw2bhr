# sub_08046914

0x08046914, 368 bytes, THUMB, parked.

Best score so far: 68.8% (best.c).

## What it does

Draws the text of an info panel at x position a for entry b of gUnknown_085D583C (20-byte records with a name id and a defence value, likely terrain types): the name centred on row 1, text 0x960 on row 3 (plus 0x969 when gUnknown_02028DD4 is 0), on row 5 either text 0x969 or the number sub_08026C6C(b) returns, and text 0x961 on row 7 if any of the three unit types listed in gUnknown_084C20C0 has a non-zero repairTable entry for b.

## How close it is

Compiles 12 bytes short (356 of 368 bytes); 14.9% of bytes line up, which means little because the size difference shifts everything after it. The call sequence, constants and every argument are settled.

## What is left

The draft keeps one value too many in registers, so it never spills: the original saves `a + 0x38` to the stack and re-reads the text-buffer pointer through its address before every call, where the draft keeps both the address and the loaded pointer. The draft's own reading is that the original had more locals here; splitting `t` or the 0x8000 and 0 constants into their own locals is the untried next step.

## Already tried

- Writing the centring as one inline expression: the compiler reorders it and loads 0x50 before the call; the separate `x = ...` statement is required.
- `(u32)` casts on the two `/ 8` divides: 8 bytes worse.
- Loading the text-buffer pointer in its own statement or inside the first call's argument: moves the load but rotates the registers of a, b and the name pointer and shrinks the frame to 8 bytes.
- Volatile or cast spellings of that load: byte-identical.

## Files

- `sub_08046914.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
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

</details>
