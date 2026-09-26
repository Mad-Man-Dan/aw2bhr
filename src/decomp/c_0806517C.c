#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806517C.
 * sub_0806517C @ 0x0806517C
 */

void sub_0806517C(struct Unk08580934_Obj *obj)
{
    gUnknown_08580934->unk2d++;
    obj->unk26 = 0xb;
    obj->unk38 = 0;
}
