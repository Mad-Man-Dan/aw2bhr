#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805C1D8.
 * CopCondTurnBoundary @ 0x0805C1D8
 */

/* The three-level `ldr rN,=<word>; ldr rM,[rN]; ldr rD,[rM]` is agbcc's own
 * -fforce-addr copy supplying the middle level, not a pointer-to-pointer: the
 * ROM word at 0x0816D9D4 holds 0x03004770, and its neighbours 0x0816D9D0 and
 * 0x0816D9D8 hold 0x03003F20 and 0x08499590, so the run is one unit's `.LC`
 * pool. CopCondUnitsInjured next door names the same global with an ordinary
 * two-level `ldr`. Naming it honestly here reproduces all three levels.
 *
 * Both compares are UNSIGNED (`bls`, `bhi`), which is what makes the counter
 * u32 rather than int. The `movs r0, #1` in the first arm is BOTH the stored
 * value and the returned one -- agbcc reuses the register it already loaded. */

int CopCondTurnBoundary(int a)
{
    if (gUnknown_03004770 > 0x10 && a == 2)
    {
        gUnknown_03004770 = 1;

        return 1;
    }

    if (gUnknown_03004770 <= 1 && a == 1)
        return 1;

    return 0;
}

asm(".global sub_0805C1D8\n.thumb_set sub_0805C1D8, CopCondTurnBoundary\n");
