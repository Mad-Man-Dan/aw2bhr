#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08056E9C.
 * sub_08056E9C @ 0x08056E9C
 */

void sub_08056E9C(u16 group, u16 index)
{
    struct Unk02029A10 *p =
        (struct Unk02029A10 *)(index * sizeof(struct Unk02029A10)
                             + group * sizeof(struct Unk02029A10Group)
                             + (u8 *)gUnknown_02029A10);

    if (p->frame != p->frameCount)
    {
        p->frame++;
        p->xSub += p->xStep;
        p->x += (s16)p->xSub >> 8;
        p->ySub += p->yStep;
        p->y += (s16)p->ySub >> 8;
    }
}
