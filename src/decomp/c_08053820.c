#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053820.
 * sub_08053820 @ 0x08053820
 */

/* Advances the two-stage flag byte, running MovePanelsForward on entry to stage 1
 * and MovePanelsBack once the 0xD0 group reads 0x50.
 *
 * The second test reuses the value the `|= 0x10` just produced rather than
 * reloading -- ordinary CSE across a plain (non-volatile) byte. */
void StepStageFlags(void)
{
    if ((gUnknown_02029664 & 3) == 1)
    {
        MovePanelsForward();
        gUnknown_02029664 |= 2;
    }

    gUnknown_02029664 |= 0x10;

    if ((gUnknown_02029664 & 0xD0) == 0x50)
    {
        MovePanelsBack();
        gUnknown_02029664 |= 0x80;
    }
}
asm(".global sub_08053820\n.thumb_set sub_08053820, StepStageFlags\n");
