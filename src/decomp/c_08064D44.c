#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064D44.
 * sub_08064D44 @ 0x08064D44
 */

void TeamBadge_SetupAtBase(struct Unk08580934_Obj *obj, int a, int index, int c)
{
    obj->unk26 = 0xb;
    obj->unk1c = index;
    obj->unk28 = a;
    obj->unk38 = 0x4e;
    obj->unk24 = c;
    gUnknown_08580934->unk44[index] = obj;
    gUnknown_08580934->unk2d++;
}
asm(".global sub_08064D44\n.thumb_set sub_08064D44, TeamBadge_SetupAtBase\n");
