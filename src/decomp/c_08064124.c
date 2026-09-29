#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064124.
 * sub_08064124 @ 0x08064124
 */

int Determinant2x2(int a, int b, int c, int d)
{
    return a * d - b * c;
}
asm(".global sub_08064124\n.thumb_set sub_08064124, Determinant2x2\n");
