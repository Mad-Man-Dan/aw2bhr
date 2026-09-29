#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E3BC.
 * sub_0805E3BC @ 0x0805E3BC
 */

void sub_0805E3BC(void)
{
    union Unk802C57CBuf v;
    int n;

    sub_0805DA84();
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 1);
    sub_0801FD9C(0x79);
    AiPickSupplyWard(&n);
    if (n == -1)
        sub_0805F7B8();
    v.pos.unk00 = gUnits[n].x;
    v.pos.unk02 = gUnits[n].y;
    AiAdvanceToward(&v);
}
