#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080650B4.
 * sub_080650B4 @ 0x080650B4
 */

/* ArmyColumnExitUp_Loop's twin: the same velocity integration, the opposite screen edge,
 * and one extra bookkeeping decrement before the same teardown call. */
void ArmyColumnExitDown_Loop(struct Unk08580934_Obj *o)
{
    o->unk3a += o->unk3c;
    o->unk2a += o->unk3a;

    if (o->unk2a > 0xA0)
    {
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }

    ArmyColumn_Draw(o);
}
asm(".global sub_080650B4\n.thumb_set sub_080650B4, ArmyColumnExitDown_Loop\n");
