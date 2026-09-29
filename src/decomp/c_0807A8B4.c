#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A8B4.
 * sub_0807A8B4 @ 0x0807A8B4
 */

void sub_0807A8B4(ProcPtr proc)
{
    int i;

    if (sub_08078E20() == 0)
        sub_0803B5A4(0);

    for (i = 0; i <= 6; i++)
        StartPalFadeToWhite(i, 0x10, proc);

    StartPalFadeToWhite(8, 0x10, proc);
    StartPalFadeToWhite(0x1b, 0x10, proc);
}
