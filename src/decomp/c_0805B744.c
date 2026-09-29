#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B744.
 * sub_0805B744 @ 0x0805B744
 */

void sub_0805B744(void)
{
    u16 pos[2];

    GenerateUnitMovementMap(gUnknown_030040D8);
    sub_0805B980();
    if (sub_0805B8F4(pos) == 1)
        sub_0805B814(pos);
    sub_0805B778();
}
