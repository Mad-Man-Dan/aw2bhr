#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022ADC.
 * sub_08022ADC @ 0x08022ADC
 */

void sub_08022ADC(void)
{
    gUnknown_030033E0.unk00 = gUnknown_030033E4.unk00 << 4;
    gUnknown_030033E0.unk02 = gUnknown_030033E4.unk02 << 4;
}
