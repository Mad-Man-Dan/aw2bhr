#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087B20.
 * sub_08087B20 @ 0x08087B20
 */

/* Draws `n` right-to-left as base-10 digits: DivRem(n, 10) picks the low digit
 * and indexes off a sprite-id base, Div(n, 10) advances, and the x coordinate
 * walks BACKWARDS eight pixels per digit.
 *
 * The `cmp r4, #0; beq` in front of the loop is the entry guard of a `while`,
 * not a `do/while` -- a zero `n` draws nothing. DrawOamObject's fifth argument
 * (the constant 2) arrives on the stack as `str r1, [sp]`. */

void DrawOamObjectNumber(int x, int y, int n, int base)
{
    while (n != 0)
    {
        DrawOamObject(base + DivRem(n, 10), x, y, 0, 2);
        n = Div(n, 10);
        x -= 8;
    }
}
asm(".global sub_08087B20\n.thumb_set sub_08087B20, DrawOamObjectNumber\n");
