#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011A64.
 * sub_08011A64 @ 0x08011A64
 */

/* A fatal-error reporter: hands the three arguments to PutAsciiStringSprites -- whose
 * declared shape `(u16, u16, const char *)` is exactly the copy-and-narrow
 * pattern here -- and then hangs in `b .` forever. The `push` has no matching
 * `pop` because control never leaves.
 *
 * The two `lsls #0x10; lsrs #0x10` pairs are agbcc's PROMOTE_MODE entry
 * zero-extension of the u16 parameters, NOT casts at the call: they are
 * emitted ahead of the argument-free `bl ClearOamShadow`, and a cast at the use
 * would sit after it. The copies into r4/r5/r6 come first because all three
 * values have to survive that call. */
void ShowMessageAndHang(u16 a, u16 b, const char *c)
{
    ClearOamShadow();
    PutAsciiStringSprites(a, b, c);

    for (;;)
        ;
}
asm(".global sub_08011A64\n.thumb_set sub_08011A64, ShowMessageAndHang\n");
