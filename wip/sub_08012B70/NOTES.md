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

## wave 97 (W97-L)
Base unchanged (87.50%, size-exact, only register numbers differ). Read sub_080726E8's copy-back step (lever 3):
it does not apply -- this function has no strength-reduction residual, the instruction stream is already 1:1.
Separate-row-base spellings compiled through spellings.py (all keep the row base in its own pseudo):
`base = dst + x + y*0x20`, `dst = dst + x; base = dst + y*0x20`, `d2 = dst` copy, `dst = base` after -- size-exact but
46.59% (the base goes to ip, dst stays in r0, so the ROM's prologue `adds r4, r0, #0` is missing: dst is never a
pseudo that outlives the stack-parameter load); `dst += x; base = dst; base += y*0x20` and `do { base = dst + ...
} while (0)` are 4 bytes short (14.77%). Nothing gives dst a prologue copy without making it the row base.
Mechanism of the tension: gcc only copies a parameter out of r0 when the pseudo is handed a callee-saved register
by global-alloc, which needs a live range beyond one block; the row-base spellings shorten dst's range to the
entry block. Not run through the permuter again (converged in wave 93).

## wave 97 (W97-AA)
Base unchanged (87.50%). Re-measured the W93 split form (`base` own local, `dst = base + i*0x20` reassigned per row) with
`spellings.py`: 84 bytes (-4). Assembly (compile_probe): the dst prologue copy IS kept (`add r4, r0, #0`) but src stays in r1
(no copy), base goes to r6 (callee-saved), w to r5, i r3, j r2. ROM: src r5 (copy), base r2 (scratch), i r1, j r3, w r6, h r7.
So the ROM's residual is an ALLOCATION ORDER fact, not a missing copy source: src's r1 preference wins in ours because src is
handed a register before `i` is; in the ROM `i` has taken r1 first, forcing src to a callee-saved copy, and base (long-lived
but call-free) sits in scratch r2. A construct must raise i's allocno priority (floor_log2(refs)*refs/live_length) above src's,
or lower src's, without changing instruction count. Not found in 3 further spellings (respell base as `dst = dst + x; base = ...`
gives 88 bytes 44%: base to ip).

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.
