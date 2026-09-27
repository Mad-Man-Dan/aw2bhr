# sub_08012B70

## Wave 93 (W93-D) -- a new structural family, and why it is 2 bytes short

The ROM's pseudo structure is finally reproduced, by REUSING THE PARAMETER
`dst` AS THE ROW POINTER and giving the row base its own local:

    base = dst + x;
    base = base + y * 0x20;
    src++;
    for (i = 0; i < h; i++) {
        dst = base + i * 0x20;          /* the parameter, reassigned */
        for (j = 0; j < w; j++) { *dst = *src + add; src++; dst++; }
    }

That gives exactly the ROM's three facts at once: `adds r4, r0, #0` (dst copied
into a callee-saved register), the row base as a SEPARATE scratch pseudo, and
the row pointer recycling dst's now-dead register. Every earlier attempt gave
the base its own local while ALSO keeping `p` separate, which let dst die
immediately and lost the prologue copy.

It is still not a match. agbcc then honours `src`'s copy-preference for its
incoming r1, keeps src there, and DROPS the `adds r5, r1, #0` prologue copy:
42 instructions against the ROM's 43, i.e. 2 bytes short. Spellings measured,
all 42 instructions with src in r1:

  * the two pointer increments in either order inside the inner loop;
  * the draft's `do { } while (0)` + `int yoff` wrapper carried over;
  * `src++` before or after the base computation;
  * parameter 2 taken as `const void *` and walked through a local `u16 *`
    -- the local folds away entirely, byte-identical to the direct form.

The complementary spelling `p = dst;` at the top (p separate, dst read-only)
is copy-propagated away: dst then stays in r0, the stack parameter loads into
r1 instead, and it is SRC that gets the prologue copy and dst that loses it.

CONCLUSION, and this is the sharpened park: the dst/base SPLIT and the TWO
prologue copies are mutually exclusive in every spelling measured. agbcc will
always leave exactly one of the two parameters in its incoming register. The
draft (fused dst/base, both copies, 43 instructions, size-exact 87.5%) is
unchanged and is still the best file.
