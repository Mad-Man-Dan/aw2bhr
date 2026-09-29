#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D990.
 * sub_0803D990 @ 0x0803D990
 */

/* A clamped step: move `a` by `b`, but stop at whichever of the two bounds `c`
 * (the low/backward one) and `d` (the high/forward one) the step would cross.
 * With `e` set, sitting exactly on a bound and stepping further wraps to the
 * other one.
 *
 * TWO spellings are load-bearing and both were measured.
 *
 * `-b` MUST be a declared local. Written inline, agbcc folds each compare into
 * `cmn r0, r1` and then cross-jumps the two arms together -- six bytes short of
 * the original and a different shape. As a local it becomes the `rsbs r2,r1,#0`
 * the ROM has, and it is also what pushes `c` out to r5 (r2 is c's incoming
 * register and the negation takes it).
 *
 * The distance test is ONE `?:` inside the `if`, not two `if`s with a `return c`
 * apiece. A conditional expression in a boolean context compiles to exactly the
 * ROM's shape -- then-arm `blt` to the shared body plus `b` past it, else-arm
 * `bge` past it and fall through -- which is why the two `cmp r0, r2`s are not
 * cross-jumped away. */
int StepClampedWithWrap(int a, int b, int c, int d, u8 e)
{
    if (b == 0)
        return a;

    if (e != 0)
    {
        if (a == c && b < 0)
            return d;

        if (a == d && b > 0)
            return c;
    }

    if (b < 0)
    {
        int n = -b;

        if (a - c >= 0 ? a - c < n : c - a < n)
            return c;
    }

    if (b > 0 && d - a < b)
        return d;

    return a + b;
}
asm(".global sub_0803D990\n.thumb_set sub_0803D990, StepClampedWithWrap\n");
