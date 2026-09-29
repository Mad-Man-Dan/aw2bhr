#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A368.
 * sub_0808A368 @ 0x0808A368
 */

/* MATCHED (wave 44, W44-G). Two counted loops around one callee with a single
 * odd call wedged between them. Both `cmp / ble` bottom tests are INCLUSIVE, so
 * the C bounds are 4 and 0x1d, not 3 and 0x1c. The argument is a ProcPtr
 * because it is forwarded unchanged into StartPalFadeToBlack's third parameter and
 * nothing narrows it. */
void sub_0808A368(ProcPtr proc)
{
    int i;

    for (i = 0; i < 4; i++)
        StartPalFadeToBlack(i, 0x10, proc);

    StartPalFadeToBlack(8, 8, proc);

    for (i = 0x10; i < 0x1d; i++)
        StartPalFadeToBlack(i, 0x10, proc);
}
