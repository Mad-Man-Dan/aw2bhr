#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A460.
 * sub_0803A460 @ 0x0803A460
 */

/* MATCHED, and the first of a byte-identical pair with sub_08047094. Clears
 * 0x400 halfwords through the gBG0TilemapBuffer pointer and restarts four
 * subsystems.
 *
 * `s16 i`, not `int` and not `u16`: the ROM re-derives the counter through
 * `lsls #0x10; asrs #0x10` at the use and keeps the incremented value as a u16
 * in the loop register, which is the s16-loop-variable shape. The pointer is
 * re-loaded (`ldr r2,[r5]`) on every iteration, so the global is not bound to a
 * local.
 *
 * THE SECOND CALL IS `sub_0801A538(0, 0, 0, 0)`, and finding that is worth more
 * than this function: sub_0801A538 was promoted as `void (void)` and takes FOUR
 * parameters. See the correction in include/unknown-functions.h. */
void sub_0803A460(void)
{
    s16 i;

    for (i = 0; i <= 0x3FF; i++)
        gBG0TilemapBuffer[i] = 0;

    sub_08013AEC();
    sub_0801A538(0, 0, 0, 0);
    RedrawUnitLayer();
    RedrawUnitIconLayer();
}
