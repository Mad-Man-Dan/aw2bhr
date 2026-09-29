#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066340.
 * sub_08066340 @ 0x08066340
 */

/* Both `movs rN, #0x80; lsls rN, #k` pairs are constants -- 0x200 and 0x100 --
 * not shifts in the source. The same wrap-and-draw shape as sub_0806A534, one
 * object along. */
void sub_08066340(struct Unk08580934_Obj *o)
{
    PutOamHi((o->unk28 + 0x200) & 0x1FF, (o->unk2a + 0x100) & 0xFF,
                 gUnknown_08580CD4, 0);
}
