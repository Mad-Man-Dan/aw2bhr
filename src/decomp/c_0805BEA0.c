#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805BEA0.
 * sub_0805BEA0 @ 0x0805BEA0
 */

/* sub_0805BE54's shape with a zero guard in front of the compare. The
 * `sub sp, #4` exists only to hold the FIFTH argument of the indirect call --
 * THUMB passes four in r0-r3 -- so there is no local in the frame. */

void sub_0805BEA0(int x, int y, u16 *out)
{
    int n;

    gUnknown_030013EC(x, y, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

    n = AiCountEnemyLandUnitsInReach();
    if (n != 0 && AiCountFriendlyArmedVehiclesInReach() < n + 5)
    {
        out[0] = x;
        out[1] = y;
    }
}
