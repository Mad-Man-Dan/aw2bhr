#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080724D4.
 * sub_080724D4 @ 0x080724D4
 */

/* Formats a signed integer into `dst` as two-byte text cells and returns the
 * cell count. gUnknown_08613F24 is the '0' cell (attribute byte, character
 * byte) and gUnknown_08613F28 the '-' cell; digits are made by adding the
 * digit value to the '0' cell's character byte.
 *
 * `n` walks BACKWARDS: the width is chosen up front from the magnitude
 * (2 bytes per digit, plus 2 for the sign), `end` snapshots it, and the
 * DivRem/Div loop fills from the last cell down. The terminator is written at
 * `end + 2` and the return is `end / 2 + 1` -- an `asrs #1`, so a shift and
 * not a division.
 *
 * The zero case spells its first two stores as `*dst++`: that is what puts
 * `adds r6, #1` between them and leaves the third store at [r6, #1]. Writing
 * dst[0]/dst[1]/dst[2] instead gives [r6]/[r6,#1]/[r6,#2] with no increment.
 * Likewise the terminator has to be `*(dst + end + 2)`: `dst[end + 2]`
 * reassociates and emits `add rD, end, dst` where the ROM has
 * `adds r1, r6, r0`. */
int NumberToStringSJis(int value, u8 *dst)
{
    int n = 0;
    int end;
    int d;

    if (value == 0)
    {
        *dst++ = gUnknown_08613F24[0];
        *dst++ = gUnknown_08613F24[1];
        *dst = 0;
        return 1;
    }

    if (value < 0)
    {
        dst[0] = gUnknown_08613F28[0];
        dst[1] = gUnknown_08613F28[1];
        value = -value;
        n = 2;
    }

    if (value > 99999)
        n += 10;
    else if (value > 9999)
        n += 8;
    else if (value > 999)
        n += 6;
    else if (value > 99)
        n += 4;
    else if (value > 9)
        n += 2;

    end = n;

    while (value > 0)
    {
        d = DivRem(value, 10);
        dst[n] = gUnknown_08613F24[0];
        dst[n + 1] = gUnknown_08613F24[1] + d;
        value = Div(value, 10);
        n -= 2;
    }

    *(dst + end + 2) = 0;

    return (end >> 1) + 1;
}
asm(".global sub_080724D4\n.thumb_set sub_080724D4, NumberToStringSJis\n");
