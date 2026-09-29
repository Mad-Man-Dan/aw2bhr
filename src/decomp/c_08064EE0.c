#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064EE0.
 * sub_08064EE0 @ 0x08064EE0
 */

void ArmyColumnMove_Init(struct Unk08580934_Obj *obj)
{
    gUnknown_08580934->unk2d++;
    obj->unk26 = 0xc;
}
asm(".global sub_08064EE0\n.thumb_set sub_08064EE0, ArmyColumnMove_Init\n");
