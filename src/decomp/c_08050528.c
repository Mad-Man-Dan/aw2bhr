#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050528.
 * sub_08050528 @ 0x08050528
 */

/* A trimmed SetSlotSpritePosition call in the same idiom as c_0804E584.c: the two
 * origin pointers come out of gUnknown_084C3F70 / gUnknown_084C3F78 and the
 * arguments are `pos - *origin` re-narrowed to s16 by the callee's prototype.
 * All four parameters are the header's declared (u16, s16, s16, s16); the four
 * `lsls #0x10; lsrs #0x10` pairs at entry are PROMOTE_MODE and say nothing
 * about signedness.
 *
 * The one lever is the gUnknown_085D6A48 ROW. Both suppression tests read the
 * same 24-byte record, and the ROM holds its ADDRESS in r1 across them
 * (`adds r1, r0, r2` then `ldrh [r1, #8]` / `ldrh [r1, #2]`). Writing the
 * subscript out twice instead keeps the table BASE in a register and adds the
 * member offset to it at each use (`adds r0, r3, #8` / `adds r0, r3, #2`),
 * which is 4 bytes long and puts v in the wrong register. Binding the row to a
 * `u16 *` is what folds the offsets into the load displacements. */
void SetEffectScreenPosition(u16 a1, s16 a2, s16 a3, s16 a4)
{
    u16 *ox;
    u16 *oy;
    u16 *e;
    s16 v;

    ox = gUnknown_084C3F70[a1];
    oy = gUnknown_084C3F78[a1];
    v = *ox;

    e = gUnknown_085D6A48[gUnknown_03004580[a1][1]];

    if (e[4] == 3)
        v = 0;
    if (e[1] == 2)
        v = 0;

    SetSlotSpritePosition(a2, a3 - v, a4 - *oy);
}
asm(".global sub_08050528\n.thumb_set sub_08050528, SetEffectScreenPosition\n");
