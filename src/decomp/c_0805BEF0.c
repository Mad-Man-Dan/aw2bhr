#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805BEF0.
 * sub_0805BEF0 @ 0x0805BEF0
 */

void sub_0805BEF0(int x, int y, u16 *out)
{
    gUnknown_030013EC(x, y, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

    if (AiCountFriendlyFootUnitsInReach() < AiScoreEnemyPropertiesInReach() + 2)
    {
        out[0] = x;
        out[1] = y;
    }
}
