#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065098.
 * sub_08065098 @ 0x08065098
 */

void ArmyColumnExitDown_Init(struct Unk08580934_Obj *obj)
{
    obj->unk3c = 2;
    obj->unk3a = 0;
    gUnknown_08580934->unk2d++;
}
asm(".global sub_08065098\n.thumb_set sub_08065098, ArmyColumnExitDown_Init\n");
