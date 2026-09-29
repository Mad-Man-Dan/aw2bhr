# sub_08073228

## Wave 93 (W93-D) -- the draft is now DEFINED C at the same bytes

The 76.4% draft was raw permuter output and read `j` in the first CpuFastSet
argument while assigning it in the second, which standard C leaves undefined.
Fixed at zero byte cost by evaluating the first argument into its own local in
the preceding statement:

    glyph = ((const u8 *) a2) + (j * 0x100);
    CpuFastSet(glyph, (void *) ((j = (i * 0x100) + 0x06010000) + (a3 * 0x20)), 0x40);

`j` is now read in one statement and written in the next, so there is no
unsequenced read and write of one object. Re-measured: 76.36%, size +0, first
difference +0x19 -- byte-identical to the old draft.

The write INTO `j` is load-bearing and must stay. Measured alternatives, all
still size-exact:
  * assign a fresh local instead of `j`:                66.8%  (-9.5)
  * two plain statements, j read then j written:        70.0%  (-6.4)
  * the same two joined by a comma operator:            70.0%  (-6.4)
Only writing into j's own pseudo gives 76.4%.

The parked entry's next lever -- recompute `k = j * 8` at the top of the goto
loop so strength reduction leaves the ROM's dead `adds r0, r4, #0` -- is
MEASURED AND WRONG, in every form tried:
  * recompute both k and the table pointer at the top:  -40 bytes, 10.0%
  * recompute k only, table pointer still stepped:       -4 bytes, 30.5%
  * the same plus the character read hoisted to a local:-12 bytes, 30.0%
  * a full clean rewrite around the recompute idea:     -12 bytes, 15.5%
Recomputing lets the compiler strength-reduce and then drop the spills that
give this function its 0x20 frame and its whole stack-slot map -- which is the
part of the draft that is already byte-exact. The dead copy has to come from
something that does not relieve register pressure.

## wave 97

Base: the wave-93 draft (76.36%). Hand probes: `c = a1[i]` bound before `j = j*8`, after it, and re-bound inside the search loop:
before/after -> 216 B (-4), 25% / 23.6% (frame 0x1c); re-bound inside the loop -> folded back to the plain draft (76.36%).
Permuter (2 runs of 900 s, `--current`): run 1 76.36 -> **82.27%, size-exact 220**, first diff +0x36; run 2 no improvement.
What run 1 changed (read, semantically identical): the search stride is held in a local (`new_var = 8;` `k += new_var; p += new_var;`),
the table base is bound to `new_var2 = gUnknown_08614024` after the loop entry and used for the `+4` advance read, and the VRAM base is
written `0x06010000 + (i * 0x100)`. Names should become `stride` / `tbl` when someone promotes it. The draft file is the 82.27% form.

## wave 97 (W97-U)

Base: the 82.27% permuter form. Found one real lever: the glyph-advance read is `*(int *)(gUnknown_08614028 + k)` in the
ROM -- a SECOND pool word for tbl+4 with offset 0 and the operand order `k + base` (`adds r0,r4,r3; ldr r1,[r0]`), not
`(tbl + k) + 4` (`ldr r1,[r0,#4]`). Declared `extern const u8 gUnknown_08614028[];` above the function (same address as
tbl+4, defined in data/data-08581E70.s) and bound `new_var2 = (const struct Unk08614024 *)gUnknown_08614028;` then
`*(int *)(((u8 *)new_var2) + k)`. That alone flipped i/k registers (76.8%); declaring `int k; int i; int j;` in that
order (k before i) restores them (80.0%). One 600 s permuter run from that: 80.00 -> 82.73% (size-exact, first diff
+0x36): it copies `a4` into a local (`new_var3 = (struct Unk73228Proc *)a4;`, used for the unk2a store) and wraps
`i = 0; acc = 4;` in `if (1)`; read: valid, wrongc "same behaviour on 400 seeds". Stack-slot order of the three hoisted
temps now matches the ROM.
Residual: the ROM loads the character once at the top of the body (`ldr r2,[sp]; adds r0,r2,r5; ... ldrb r3,[r0]`)
BEFORE `k`/`p` are set up, and compares that register; the draft sets `k`/`p` first and loads the char after. Also two
commuted adds (`adds r1,r3,r1` glyph destination; `adds r0,r4,r3` advance read). Tried: `(a3*0x20) + (j = ...)` 70.5%,
`k + (u8*)new_var2` byte-neutral, `c = a1[i]` bound at the loop top: 21.4% -4 (frame 0x1c).
Proposed summary: left = "loop-top ordering of the character load vs the k/p setup, two operand orders".
