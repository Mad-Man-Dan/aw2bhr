#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035584.
 * sub_08035584 @ 0x08035584
 */

#include "proc.h"
/* Spawns the unit sprite for one gUnits record and hands the record
 * back to it.
 *
 * The `x * 5, * 17, * 257, * 65537` shift-add chain followed by `rsbs` is ONE
 * operation: agbcc's exact division for a pointer subtraction whose element
 * size is 0x0c. `-0x55555555` is `0xAAAAAAAB`, the modular inverse of 3, and
 * the remaining `>> 2` for the factor of 4 is folded into the source's own
 * `>> 6` to give the `lsls #8; lsrs #0x10` pair -- which is that `>> 8` and the
 * u16 conversion of the third argument fused. So the whole thing is
 * `(p - gUnits) >> 6`, the record index divided by the 64-entry
 * army group that unknown-globals.h documents.
 *
 * The cast is the struct Unk030040D8 / struct Unit duplication the note
 * on the former describes: the two are the same object and the difference has
 * to be taken in one of them.
 *
 * It RETURNS the proc -- the declaration said `void`; see unknown-functions.h. */
struct Unk35584Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x30);
    /* 0x30 */ int unk30;
};

ProcPtr CreateMoveSlideForActiveUnit(struct Unk030040D8 *p)
{
    struct Unk35584Proc *proc;

    p->unk01 |= 6;
    proc = CreateMoveSlide(p->unk02, p->unk03,
                        ((struct Unit *)p - gUnits) >> 6,
                        p->unk00);
    if (proc == NULL)
        return NULL;
    else
    {
        proc->unk30 = (int)p;
        return proc;
    }
}
asm(".global sub_08035584\n.thumb_set sub_08035584, CreateMoveSlideForActiveUnit\n");
