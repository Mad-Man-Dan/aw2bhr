#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065E5C.
 * sub_08065E5C @ 0x08065E5C
 */

#include "hardware.h"

/* Per-slot tick: for every entry whose unk70 mark is clear, hand MatchSetupCycleCo
 * that slot's key state and whether the slot is the one unk25 selects.
 *
 * gUnknown_03002040 is the same 20-byte-per-slot KeySt array c_08064410.c
 * indexes (0x14 stride, `ldrh [rN, #2]` = KeySt.repeated).
 *
 * The base binding MUST be block-scoped INSIDE the loop body. Written that way
 * it is loop-invariant, LICM hoists it into the preheader AFTER the
 * zero-trip guard, and strength_reduce folds it into the giv -- which is the
 * ROM's `ldr r5,=gUnknown_03002040` sitting after `adds r6,r1,#0`, and it puts
 * gUnknown_08580934 first in the pool because the guard references it first.
 * Bound at FUNCTION scope before the loop the pool order reverses and the load
 * moves ahead of the guard (+2 bytes); written inline as
 * `((struct KeySt *)&gUnknown_03002040)[i]` the base is not folded into the giv
 * at all and is rematerialised inside the `if`. All three probed. */
void MatchSetupCycleCosByOwnKeys(void)
{
    int i;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        struct KeySt *ks = (struct KeySt *)&gUnknown_03002040;

        if ((s8)gUnknown_08580934->unk70[i] == 0)
            MatchSetupCycleCo(i, ks[i].repeated, i == gUnknown_08580934->unk25);
    }
}
asm(".global sub_08065E5C\n.thumb_set sub_08065E5C, MatchSetupCycleCosByOwnKeys\n");
