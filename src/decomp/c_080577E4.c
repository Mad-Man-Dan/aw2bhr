#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080577E4.
 * sub_080577E4 @ 0x080577E4
 */

struct Unk8057Pos
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};

void DrawHpNumber(u16 *dst, int i, struct Unk8057Pos *pos)
{
    int j = i & 1;
    int k = (gUnknown_02029B78[j] + 9) / 10;

    sub_080718F8(dst + ((gUnknown_08553A28 + i)->unk00 + pos->x
                        + (((gUnknown_08553A28 + i)->unk01 + pos->y) << 5)),
                 gUnknown_08553AD8[k],
                 (u16)(gUnknown_08562124[j] * 0x1000));
}
asm(".global sub_080577E4\n.thumb_set sub_080577E4, DrawHpNumber\n");
