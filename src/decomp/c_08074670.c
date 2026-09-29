#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074670.
 * sub_08074670 @ 0x08074670
 */

#include "proc.h"
/* Walks the 12-byte record list at &gUnknown_0202FDFC.unk3c and redraws every
 * record whose owner bit is set and whose screen-relative position is inside
 * the view. The record base and the struct base are ONE symbol: the ROM
 * materialises 0x0202FE38 in a single pool word and derives 0x0202FDFC with
 * `adds/subs #0x3c`, which agbcc can only do for one symbol_ref plus a
 * constant. See gUnknown_0202FE38 in include/unknown-globals.h.
 * The x pair folds to one unsigned compare and the y pair does not because
 * `a && b && c && d` is left-associated: fold_range_test only ever sees the
 * first two comparisons adjacent, so only they merge.
 */
struct Unk8074670Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void WorldMapMarkers_Draw(struct Unk8074670Proc *proc)
{
    struct Unk0202FE38 *p;

    for (p = (struct Unk0202FE38 *)&gUnknown_0202FDFC.unk3c; p->unk00 != -1; p++)
    {
        if (gUnknown_0202FDFC.unk12[p->unk00] & 1)
        {
            int x = p->unk02 - gUnknown_0202FDFC.unk00;
            int y = p->unk04 - gUnknown_0202FDFC.unk02;

            if (x >= -0x10 && x <= 0xf0 && y >= -0x10 && y <= 0xa0)
            {
                p->unk08->unk22 = (p->unk08->unk22 & 0xf3ff) | proc->unk58;
                AP_Update(p->unk08, x & 0x1ff, y & 0xff);
            }
        }
    }
}
asm(".global sub_08074670\n.thumb_set sub_08074670, WorldMapMarkers_Draw\n");
