#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B91C.
 * sub_0808B91C @ 0x0808B91C
 */

/* WAVE 76: MATCHED. Declaring the final `v` as a fixed r4 register local is
 * sufficient to reproduce the ROM's three remaining moves. Its lifetime does
 * not cross a call, and it safely reuses the register occupied by the earlier
 * reduced-x value after that value dies.
 *
 * PARKED at 99.4% -- 496 bytes, size-exact, THREE bytes differ, all of them
 * the same register-allocation choice. See data/parked.json.
 *
 * cosf. Range-reduce x by whole multiples of pi, evaluate the cosine Taylor
 * series in u = r*r out to the r^16 term, negate on odd n.
 *
 * What this function establishes about soft float is written up in the
 * "Soft float" chapter of docs/agbcc-codegen.md, and is worth more than the
 * three bytes: the double word order in registers is HIGH FIRST (FPA layout),
 * every `__*df3` here comes from an UNSUFFIXED decimal constant promoting a
 * float operand (`u * 0.5f` in the same function emits `__mulsf3`), `t - C`
 * emits `__subdf3` so an alternating series needs explicit negative literals
 * with `+`, and the coefficients are 13-significant-digit hand-typed decimals
 * that `1.0/24.0` does not reach.
 *
 * THE REMAINING DIFF, exactly: `v` is allocated r1 and the ROM allocates it
 * r4. Three instructions, no others differ:
 *     ROM  adds r4, r0, #0  /  adds r0, r4, #0  x2
 *     here adds r1, r0, #0  /  adds r0, r1, #0  x2
 * gcc's REG_ALLOC_ORDER puts the call-clobbered registers first and `v`
 * crosses no call, so r1 is taken before r4 is considered. For the ROM to
 * pick r4, `v` has to cross a call -- but `v = -v;` (which would make it do
 * so) reshapes the tail and drops the whole function to 75.2%.
 *
 * The `do { } while (0)` on line 3 of the body is a DIAGNOSTIC, not a claim
 * about the original source. Without it the same file scores 97.6% with
 * twelve differing bytes: `x` lands in r6 and the x/pi quotient takes r4:r5,
 * where the ROM has x in r4 and the quotient in r5:r6. The wrapper flips that
 * because gcc weights allocno priority by loop depth, so everything inside a
 * zero-trip loop outranks everything outside it -- which is a real lever
 * (recorded in docs/agbcc-codegen.md) but almost certainly reflects the
 * original having been written through a macro rather than a literal
 * do/while. If a cleaner way to raise x's priority turns up, drop the wrapper
 * and re-check both halves at once.
 *
 * Ruled out. Toolchain: default, old_agbcc without -fprologue-bugfix, no
 * -fforce-addr, and both together all give the IDENTICAL 99.4% and the same
 * three bytes -- tested by monkeypatching agbenv._OVERRIDES, not by editing
 * data/compiler-overrides.json. Source: two 300 s permuter runs (the second
 * from this base, which is how the do/while was found) and hand sweeps over
 * the return form (`v = -v`, `?:`, explicit else, `n % 2`, `0.0f - v`,
 * `n & 1` hoisted to a local), local declaration order, binding the quotient
 * to a `double`, wrapping the tail and the return in their own do/while, and
 * reusing the parameter for the square as well as the reduced value (+12
 * bytes -- one long live range instead of two that share r4).
 */
float cosf(float x)
{
    int n;
    float u;
    float p;
    register float v asm("r4");

    n = x / 3.141592653589793 + (x >= 0 ? 0.5 : -0.5);
    do { x = x - n * 3.141592653589793; u = x * x; } while (0);

    p = (u * u)
        * (u * (u * (u * (u * (u * (u * 4.779477332387e-14
                                    + -1.147074559773e-11)
                                + 2.087675698787e-9)
                           + -2.755731922399e-7)
                      + 2.480158730159e-5)
                 + -0.001388888888889)
           + 0.04166666666667);

    v = 1.0 - u * 0.5f + p;

    if (n & 1)
        return -v;

    return v;
}
asm(".global sub_0808B91C\n.thumb_set sub_0808B91C, cosf\n");
