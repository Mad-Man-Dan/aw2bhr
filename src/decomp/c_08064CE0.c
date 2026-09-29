#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064CE0.
 * sub_08064CE0 @ 0x08064CE0
 */

void TeamBadgeExitDown_Init(struct Unk08580934_Obj *obj)
{
    gUnknown_08580934->unk2d++;
    obj->unk3c = 2;
    obj->unk3a = 0;
}
asm(".global sub_08064CE0\n.thumb_set sub_08064CE0, TeamBadgeExitDown_Init\n");
