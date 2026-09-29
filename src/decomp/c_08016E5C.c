#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016E5C.
 * sub_08016E5C @ 0x08016E5C
 */

/* The literal pool sits between the two return blocks, which is the
 * `if (C) return A; return B;` shape rather than a ternary (whose pool would
 * follow the `bx lr`) or a plain `return !(x & 1);` (which would never
 * materialise both constants). */
int IsProfileNeverSaved(void)
{
    if (gUnknown_0200C420.unk08 & 1)
        return 0;
    return 1;
}
asm(".global sub_08016E5C\n.thumb_set sub_08016E5C, IsProfileNeverSaved\n");
