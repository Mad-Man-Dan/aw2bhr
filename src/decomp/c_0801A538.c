#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A538.
 * sub_0801A538 @ 0x0801A538
 */

/* Family F005 (data/families.json): 19 sixteen-byte wrappers shaped
 * `push {lr}; bl S; bl S; pop {r0}; bx r0` -- two calls, nothing between
 * them, `pop {r0}` so void.
 * Two statements and not `g(f())`: the second callee takes no arguments (it
 * opens with a `bl`, a pool `ldr` or a `movs` into r0, never a read of r0),
 * so there is no parameter for the first call's result to reach. `g(f())`
 * would be byte-identical only if `g` had a parameter, and none of the 19
 * second callees does -- see the F005 block in unknown-functions.h.
 *
 * RETYPED IN WAVE 20 (W20-C): this function takes FOUR parameters, and none of
 * them is used. `(void)` was the F005 default and was never argued from
 * evidence -- the comment above is about the SECOND CALLEE's arity, not this
 * function's. The callers settle it, which is the standard this tree uses for
 * a signature: all four `bl sub_0801A538` sites in asm/ set r0-r3 immediately
 * before the call, and sub_08019DA8 sets them to 0, 1, 6 and 0xC -- four
 * distinct non-zero constants, which nothing but four arguments explains. The
 * three in code-0801D390.s (sub_0803A460, sub_0803A518, sub_08047094) pass
 * four zeros.
 *   Unused parameters cost nothing here: they arrive in r0-r3 and the body
 * overwrites r0 with the first `bl`, so this definition is byte-identical
 * before and after the retype (re-verified with trymatch). `int` is the
 * weakest type that fits the constants seen. */

void sub_0801A538(int a, int b, int c, int d)
{
    sub_080199F8();
    sub_08024584();
}
