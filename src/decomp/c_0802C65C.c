#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C65C.
 * sub_0802C65C @ 0x0802C65C
 */

/* `movs r0, #0; bx lr` -- a leaf with no frame that returns a constant zero.
 *
 * `int` is the weakest type that fits and the return type is NOT proved: the
 * body materialises a literal 0, which is byte-identical for int, bool8 and
 * every narrower type, and nothing in the ROM calls this to re-narrow the
 * result. Its neighbour IsMapCategoryZero is a declared bool8 predicate, so a stub
 * "always false" is the likely reading, but that is a guess and is recorded as
 * one.
 */

int OptionsMenu_YieldUsability(void)
{
    return 0;
}
asm(".global sub_0802C65C\n.thumb_set sub_0802C65C, OptionsMenu_YieldUsability\n");
