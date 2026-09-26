#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080650FC.
 * sub_080650FC @ 0x080650FC
 */

void sub_080650FC(struct Unk08580934_Obj *obj)
{
    gUnknown_08580934->unk2d++;
    obj->unk26 = 0xb;
    obj->unk38 = 0;
}
