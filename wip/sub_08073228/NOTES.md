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
