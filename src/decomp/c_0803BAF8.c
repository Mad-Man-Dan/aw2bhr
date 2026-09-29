#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BAF8.
 * sub_0803BAF8 @ 0x0803BAF8
 */

/* `movs r0, #1; bx lr` -- a leaf that returns a constant 1 and reads nothing.
 * Exactly 4 bytes with no padding. The return type is not recoverable: `bool8`,
 * `int`, `u8` and `u16` all emit the same `movs r0, #1` for a returned literal,
 * with no narrowing either way. `bool8` is the reading the rest of this tree
 * uses for a constant-TRUE proc predicate (src/decomp/c_08015E58.c and its
 * neighbours), and the three identical stubs at 0x0803BAF8, 0x0803BB8C and
 * 0x0803BBA4 all sit in the proc-callback runs. The argument count is a floor
 * of zero and nothing raises it. */

bool8 MainMenu_UsabilityAlways(void)
{
    return TRUE;
}
asm(".global sub_0803BAF8\n.thumb_set sub_0803BAF8, MainMenu_UsabilityAlways\n");
