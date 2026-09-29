#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063A58.
 * sub_08063A58 @ 0x08063A58
 */

void SplitDecimalDigits(int val, u8 *hundreds, u8 *tens, u8 *ones)
{
    if (val == 0)
    {
        *ones = 10;
        *tens = 10;
        *hundreds = 0xFF;
    }
    else
    {
        *ones = val % 10;
        val /= 10;
        *tens = val % 10;
        val /= 10;
        *hundreds = val % 10;
        if (*hundreds == 0)
        {
            if (*tens == 0)
                *tens = 0xFF;
            if (*hundreds == 0)
                *hundreds = 0xFF;
        }
    }
}
asm(".global sub_08063A58\n.thumb_set sub_08063A58, SplitDecimalDigits\n");
