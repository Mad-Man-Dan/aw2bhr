#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016A2C.
 * sub_08016A2C @ 0x08016A2C
 */

/* GetSuspendIdForGameMode's inverse (1,2,4 -> 0,1,2 where that one maps 1,2,3 -> 2,3,4),
 * and a `switch`, not an if/else chain: the repeated `cmp r0,#2` with `beq`
 * then `bgt` is gcc's binary-search decision tree. The parameter is NOT
 * narrowed at entry, so it is `int` and not the `u8` GetSuspendIdForGameMode takes. The
 * function has no callers in the tree, so the `s8` return is by analogy with
 * GetSuspendIdForGameMode's -- the constants are byte-identical under any width. */
s8 sub_08016A2C(int a)
{
    switch (a) {
    case 1:
        return 0;
    case 2:
        return 1;
    case 4:
        return 2;
    }
    return -1;
}
