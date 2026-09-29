#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A550.
 * sub_0803A550 @ 0x0803A550
 */

/* MATCHED, wave 43 (W43-I), first attempt.
 *
 * Blanks the whole 0x400-entry halfword buffer gBG0TilemapBuffer points at,
 * then kicks the four consumers.
 *
 * THE COUNTER IS `s16`, and the loop bottom says so in one shot: the increment
 * keeps BOTH halves -- `lsrs r1, r0, #0x10` for the next iteration's value and
 * `asrs r0, r0, #0x10` for the `<= 0x3ff` compare -- while the body re-reads it
 * `lsls #0x10; asrs #0x10`. A `u16` gives `lsrs` in both places and an `int`
 * gives neither pair.
 *
 * gBG0TilemapBuffer is a POINTER global and is re-`ldr`ed every iteration
 * (`ldr r2, [r5]` inside the loop). Do not bind it to a local -- same rule as
 * gBG1TilemapBuffer in c_08037B90.c. */
void sub_0803A550(void)
{
    s16 i;

    sub_08014878();

    for (i = 0; i <= 0x3ff; i++)
        gBG0TilemapBuffer[i] = 0;

    BG_EnableSyncBG0();
    DisableWindow0AndResetMapLayers(0, 0, 0, 0);
    RedrawUnitLayer();
    RedrawUnitIconLayer();
}
