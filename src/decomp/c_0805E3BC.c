#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E3BC.
 * sub_0805E3BC @ 0x0805E3BC
 */

void AiDeliberateSupply(void)
{
    union Unk802C57CBuf v;
    int n;

    AiDeliberateSupplyInReach();
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 1);
    MapMarkHalo(0x79);
    AiPickSupplyWard(&n);
    if (n == -1)
        AiFallbackMove();
    v.pos.unk00 = gUnits[n].x;
    v.pos.unk02 = gUnits[n].y;
    AiAdvanceToward(&v);
}
asm(".global sub_0805E3BC\n.thumb_set sub_0805E3BC, AiDeliberateSupply\n");
