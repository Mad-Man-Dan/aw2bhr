#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BBA4.
 * sub_0803BBA4 @ 0x0803BBA4
 */

/* `movs r0, #1; bx lr` -- a leaf returning a constant 1, exactly 4 bytes with
 * no padding, and one of three identical copies in this block with
 * MainMenu_UsabilityAlways and sub_0803BB8C. The return type is not recoverable: a returned
 * literal 1 is the same `movs r0, #1` for bool8/int/u8/u16 with no narrowing.
 * `bool8` follows the constant-TRUE proc predicates elsewhere in the tree
 * (src/decomp/c_08015E58.c). */

bool8 sub_0803BBA4(void)
{
    return TRUE;
}
