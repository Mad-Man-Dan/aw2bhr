#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064CE0.
 * sub_08064CE0 @ 0x08064CE0
 */

void sub_08064CE0(struct Unk08580934_Obj *obj)
{
    gUnknown_08580934->unk2d++;
    obj->unk3c = 2;
    obj->unk3a = 0;
}
