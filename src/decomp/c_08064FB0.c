#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064FB0.
 * sub_08064FB0 @ 0x08064FB0
 */

void ArmyColumnEnter_Init(struct Unk08580934_Obj *obj)
{
    gUnknown_08580934->unk2d++;
    obj->unk26 = 0xe;
}
asm(".global sub_08064FB0\n.thumb_set sub_08064FB0, ArmyColumnEnter_Init\n");
