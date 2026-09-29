#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C27C.
 * sub_0801C27C @ 0x0801C27C
 */

/* Draws the handle's current frame. THREE parameters: r1 and r2 are copied out
 * at entry and go straight to PutSpriteExt as the x|flags word (OR-ed with the
 * priority nibble) and the y word. Its one caller, AP_Update, forwards its
 * own a2/a3 untouched, which is why the arity is invisible there -- the header
 * declaration was corrected this wave and c_0801C254.c re-verified.
 *
 * `flags` is initialised BEFORE the two null tests, which is where the ROM puts
 * `movs r2, #0` -- it is live across the merge at the PutSpriteExt call, so the
 * declaration-order statement is what places it. Nothing is returned: the exit
 * label is reached by three paths and none of them sets r0. */
void AP_Display(struct Unk0801C210 *a1, int a2, int a3)
{
    int flags;

    flags = 0;
    if (a1 == NULL || a1->unk00 == NULL)
        return;

    if (a1->unk14 != NULL)
    {
        AP_QueueObjRotScale(a1);
        flags = a1->unk21 << 9;
    }

    PutSpriteExt(a1->unk1e, flags | a2, a3, a1->unk10, a1->unk22);

    if (a1->unk24 != NULL && (a1->unk20 & 1))
        AP_QueueObjGraphics(a1);
}
asm(".global sub_0801C27C\n.thumb_set sub_0801C27C, AP_Display\n");
