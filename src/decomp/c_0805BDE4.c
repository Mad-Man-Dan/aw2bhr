#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805BDE4.
 * sub_0805BDE4 @ 0x0805BDE4
 */

/* The two stack locals are pure OUT-PARAMETERS: AiFindEnemyHqInInterestList writes a word
 * through each and nothing here reads them back, which is the whole reason for
 * the 8-byte frame. sub_0805B4A8 takes no arguments -- r0/r1/r2 still holding
 * this function's parameters at that `bl` is an artefact of the copies into
 * r4/r5/r6 above it, not an argument pass. */

void sub_0805BDE4(int x, int y, u16 *out)
{
    int a;
    int b;

    if (AiFindEnemyHqInInterestList(sub_0805B4A8(), &a, &b) == 1)
    {
        out[0] = x;
        out[1] = y;
    }
}
