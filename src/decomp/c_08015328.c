#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015328.
 * sub_08015328 @ 0x08015328
 */

/* Tears a gUnknown_03001470 slot down: clear its script pointer, run the
 * slot's +0x0c callback with gUnknown_03001FBC pointed at this slot, and if
 * flag bit 1 is set release the sprite the slot owns.
 *
 * gUnknown_03001FBC is saved and restored around the callback exactly as
 * sub_08015224/sub_0801527C do it, and for the same reason the `ldrh`/`strh`
 * pair is plain rather than `ldrsh`: an s16 read whose only use is an s16
 * store is a pure HImode move.
 *
 * The slot index is used BOTH ways and the two shift results are both live:
 * `lsrs` (zero-extended) is what `gUnknown_03001FBC = a` stores, `asrs`
 * (sign-extended) is what the array subscript and DisableSlotSpriteAffine need. That is
 * PROMOTE_MODE on an s16 parameter, not two variables.
 *
 * `(s16)` on the u16 member ->unk26 folds into the `ldrsh` and costs nothing,
 * unlike the same cast on a parameter or local; retyping the member is not
 * needed and would touch its other users.
 */
void EndSlotScriptAt(s16 a)
{
    s16 saved = gUnknown_03001FBC;

    gUnknown_03001470[a].unk00 = 0;

    if (gUnknown_03001470[a].unk0c != 0)
    {
        gUnknown_03001FBC = a;
        ((void (*)(struct Unk03001470 *))gUnknown_03001470[a].unk0c)(&gUnknown_03001470[a]);
    }

    if (gUnknown_03001470[a].unk12 & 2)
    {
        DisableSlotSpriteAffine(a);
        FreeSpriteScript2((s16)gUnknown_03001470[a].unk26);
    }

    gUnknown_03001FBC = saved;
}
asm(".global sub_08015328\n.thumb_set sub_08015328, EndSlotScriptAt\n");
