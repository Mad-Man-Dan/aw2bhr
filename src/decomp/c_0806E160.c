#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E160.
 * sub_0806E160 @ 0x0806E160, sub_0806E17C @ 0x0806E17C
 */

/* Family F054, the gUnknown_08581F7C twin of StartMatchSetupScreen -- same shape, same
 * constants, different flag and script. See the note on StartMatchSetupScreen. */

void sub_0806E160(void)
{
    gUnknown_0202F2C8 = 0;
    sub_080152EC(gUnknown_08581F7C, 2);
}

/* Family F054, the mode-1 half of the sub_0806E160 pair. See the note on
 * StartMatchSetupScreen. */

void sub_0806E17C(void)
{
    gUnknown_0202F2C8 = 1;
    sub_080152EC(gUnknown_08581F7C, 2);
}
