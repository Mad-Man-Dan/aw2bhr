#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804DC5C.
 * sub_0804DC5C @ 0x0804DC5C
 */

/* The plainest member of the ~40 sprite setters between 0x0804B180 and
 * 0x08053614: it re-points one gUnknown_02029A10 entry, taking x as-is and
 * subtracting only the second scroll origin from y.
 *
 * The scroll value is bound to a local because the ROM reads it FIRST, before
 * the entry address is built -- the same ordering lever c_0804B2A8.c documents
 * at length. x carries an explicit `(s16)` cast: the member is `u16` (see the
 * note on struct Unk02029A10) and the ROM reads it `ldrsh` through the
 * register-offset form, so the sign extension is at this use. y needs no cast --
 * the subtraction lands in an `s16` parameter, which is where its
 * `lsls #0x10; asrs #0x10` comes from. */
void RidePartOnFigure(u16 a, u16 b, int c)
{
    u16 v;

    v = *gUnknown_084C3F78[a];

    SetSlotSpritePosition(c, (s16)gUnknown_02029A10[a].entries[b].x,
                 gUnknown_02029A10[a].entries[b].y - v);
}
asm(".global sub_0804DC5C\n.thumb_set sub_0804DC5C, RidePartOnFigure\n");
