#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080500D0.
 * sub_080500D0 @ 0x080500D0
 */

/* Both ROM origin pointers are dereferenced into locals BEFORE either
 * coordinate is read: the ROM's pool words come out gUnknown_084C3F70,
 * gUnknown_084C3F78, gUnknown_02029C0C, gUnknown_02029C10 and it keeps the two
 * loaded pointers live in r4/r5, where the inline spelling interleaves the four
 * addresses one per argument. */
void sub_080500D0(void)
{
    u16 side;
    u16 *p;
    u16 *q;

    side = gUnknown_03001470[gUnknown_03001FBC].unk30;
    p = gUnknown_084C3F70[side];
    q = gUnknown_084C3F78[side];

    SetSlotSpritePosition(gUnknown_03001FBC, gUnknown_02029C0C[side] - *p,
                 gUnknown_02029C10[side] - *q);
}
