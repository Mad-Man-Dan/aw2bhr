#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035CF4.
 * sub_08035CF4 @ 0x08035CF4
 */

/* The `adds r0, #0x2c` ahead of the `ldrb` is the THUMB ldrb displacement limit
 * (0-31), not an address being taken. */
/* Wave 37 (W37-H): retyped `int (void)` -> `u8 (ProcPtr)`. The one caller
 * GetMoveSlideFootstepSfx loads r0 with the proc immediately before the `bl` (so the
 * argument is real, even though this body ignores it) and narrows the result
 * with `lsls r0,#0x18`. Byte-exact before and after. */
u8 IsWeatherSnowing(ProcPtr proc)
{
    if (gPlaySt.weather == 1)
        return 1;

    return 0;
}
asm(".global sub_08035CF4\n.thumb_set sub_08035CF4, IsWeatherSnowing\n");
